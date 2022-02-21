//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"
#include "logging/Logger.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator) : gen_(generator) {
}

void TaskManager::AddChild(const Core::TaskID &parent_id, const Core::TaskID &child_id) {
    auto it = tasks_.find(parent_id);
    if (it != tasks_.end()) {
        it->second.second.AddChild(child_id);
        LOG_STREAM(debug) << "Marked " << child_id.value() << " as child of " << parent_id.value() << ".";
    }
}

void TaskManager::RemoveChild(const Core::TaskID &parent_id, const Core::TaskID &child_id) {
    auto it = tasks_.find(parent_id);
    if (it != tasks_.end()) {
        it->second.second.RemoveChild(child_id);
        LOG_STREAM(debug) << "Removed " << child_id.value() << " from children of " << parent_id.value() << ".";
    }
}

std::optional<Core::Task> TaskManager::GetTask(const Core::TaskID &id) const {
    auto it = tasks_.find(id);
    if (it != tasks_.end())
        return it->second.first;
    else
        return std::nullopt;
}

std::vector<Core::TaskID> TaskManager::ChildrenOf(const Core::TaskID &parent_id) const {
    auto it = tasks_.find(parent_id);
    if (it != tasks_.end())
        return it->second.second.children();
    else
        return std::vector<Core::TaskID>{};
}

std::optional<Core::TaskID> TaskManager::ParentOf(const Core::TaskID &id) const {
    auto it = tasks_.find(id);
    if (it != tasks_.end())
        return it->second.second.parent();
    else
        return std::nullopt;
}

Core::ModelRequestResult TaskManager::Add(const Core::Task &t) {
    Core::ModelRequestResult result;
    Core::TaskID id = gen_->genID();

    std::lock_guard lk{mtx_};

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node())));
    LOG_STREAM(debug) << "Added task with ID: " << id.value() << " " << t.ShortDebugString();
    result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());

    return result;
}

Core::ModelRequestResult TaskManager::AddSubtask(const Core::Task &t, const Core::TaskID &parent) {
    Core::ModelRequestResult result;
    auto id = gen_->genID();

    std::lock_guard lk{mtx_};
    if (!ToBool(IsPresent(parent)))
        result.set_status(Core::ModelRequestResult_Status_PARENT_ID_NOT_FOUND);
    else {
        tasks_.insert(std::make_pair(id, std::make_pair(t, Node(parent))));
        AddChild(parent, id);
        result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    }

    LOG_STREAM(debug) << "Added Subtask of " << parent.value() << " with ID: " << id.value() << " " << t.ShortDebugString();

    return result;
}

std::vector<Core::TaskEntity> TaskManager::getTasks() const {
    std::vector<Core::TaskEntity> vec;

    std::lock_guard lk{mtx_};
    for (const auto &kv : tasks_) {
        if (kv.second.second.parent()) {
            vec.emplace_back(Core::createTaskEntity(kv.first, kv.second.first, *kv.second.second.parent()));
        } else {
            vec.emplace_back(Core::createTaskEntity(kv.first, kv.second.first));
        }
    }
    LOG_STREAM(debug) << "Returned " << vec.size() << " tasks.";

    return vec;
}

std::vector<Core::TaskEntity> TaskManager::getTasks(const std::string &label) const {
    std::vector<Core::TaskEntity> tasks;

    std::lock_guard lk{mtx_};
    for (const auto &kv : tasks_) {
        for (const auto &task_label : kv.second.first.labels()) {
            if (task_label == label) {
                tasks.emplace_back(Core::createTaskEntity(kv.first, kv.second.first));
                break;
            }
        }
    }

    LOG_STREAM(debug) << "Returned " << tasks.size() << " tasks with label " << label << ".";

    return tasks;
}

std::vector<Core::TaskEntity> TaskManager::getTaskWithSubtasks(const Core::TaskID &id) const {
    std::vector<Core::TaskEntity> tasks;
    std::optional<Core::Task> task;
    {
        std::lock_guard lk{mtx_};
        task = GetTask(id);
    }

    if (task)
        tasks.emplace_back(Core::createTaskEntity(id, *task));
    else {
        LOG_STREAM(debug) << "Task with ID " << id.value() << " not found.";
        return tasks;
    }

    {
        std::lock_guard lk{mtx_};
        // not including parent, but will include children
        for (const auto &ch_id: ChildrenOf(id)) {
            auto ch_tasks = getTaskWithSubtasks(ch_id);
            for (auto &ch_task: ch_tasks) {
                ch_task.mutable_parent()->CopyFrom(*ParentOf(ch_task.id()));
            }
            tasks.insert(tasks.end(), ch_tasks.begin(), ch_tasks.end());
        }
    }

    LOG_STREAM(debug) << "Returned task " << id.value() << " with " << tasks.size() - 1 << " subtasks.";

    return tasks;
}

Core::ModelRequestResult TaskManager::Delete(const Core::TaskID &id, bool deleteChildren) {
    Core::ModelRequestResult result;

    std::lock_guard lk{mtx_};
    if (ToBool(IsPresent(id))) {
        if (!ChildrenOf(id).empty() && !deleteChildren) {
            LOG_STREAM(debug) << "Failed to delete task (has children).";
            result.set_status(Core::ModelRequestResult_Status_HAS_CHILDREN);
        } else {
            std::optional<Core::TaskID> ancestor = ParentOf(id);
            if (ancestor && ToBool(IsPresent(*ancestor)))
                RemoveChild(*ancestor, id);

            for (auto const &ch: ChildrenOf(id))
                Delete(ch, true);

            tasks_.erase(id);
            LOG_STREAM(debug) << "Erased task with ID " << id.value();
            result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
        }
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

Core::ModelRequestResult TaskManager::Edit(const Core::TaskID &id, const Core::Task &t) {
    std::lock_guard lk{mtx_};

    Core::ModelRequestResult result;
    auto it = tasks_.find(id);

    if (it != tasks_.end()) {
        it->second.first = t;
        LOG_STREAM(debug) << "Edited task with ID " << id.value();
        result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

Core::ModelRequestResult TaskManager::Complete(const Core::TaskID &id) {
    std::lock_guard lk{mtx_};
    Core::ModelRequestResult result;
    auto it = tasks_.find(id);

    if (it != tasks_.end()) {
        it->second.first.set_is_complete(true);
        LOG_STREAM(debug) << "Marked " << id.value() << " as completed.";
        for (auto const &ch : ChildrenOf(id))
            Complete(ch);

        result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

Core::ModelRequestResult TaskManager::Uncomplete(const Core::TaskID &id) {
    Core::ModelRequestResult result;
    std::lock_guard lk{mtx_};
    auto it = tasks_.find(id);

    if (it != tasks_.end()) {
        it->second.first.set_is_complete(false);
        LOG_STREAM(debug) << "Marked " << id.value() << " as not completed.";
        for (auto const &ch : ChildrenOf(id))
            Uncomplete(ch);
        result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

Core::ModelRequestResult TaskManager::IsPresent(const Core::TaskID &id) const {
    Core::ModelRequestResult result;
    std::lock_guard lk{mtx_};

    if (tasks_.find(id) != tasks_.end())
        result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    else
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);

    return result;
}

size_t TaskManager::size() const {
    std::lock_guard lk{mtx_};
    return tasks_.size();
}

Core::ModelRequestResult TaskManager::AddLabel(const Core::TaskID &id, const std::string &label) {
    Core::ModelRequestResult result;
    std::lock_guard lk{mtx_};
    auto it = tasks_.find(id);

    if (it != tasks_.end()) {
        auto labels = it->second.first.labels();
        if (find(labels.begin(), labels.end(), label) == labels.end()) {
            it->second.first.add_labels(label);
            LOG_STREAM(debug) << "Added " << label << "label to " << id.value() << ".";
            result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
        }
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

Core::ModelRequestResult TaskManager::RemoveLabel(const Core::TaskID &id, const std::string &label) {
    Core::ModelRequestResult result;
    std::lock_guard lk{mtx_};
    auto it = tasks_.find(id);

    if (it != tasks_.end()) {
        auto labels = it->second.first.mutable_labels();
        auto label_it = find(labels->begin(), labels->end(), label);
        if (label_it != labels->end()) {
            it->second.first.mutable_labels()->erase(label_it);
            LOG_STREAM(debug) << "Removed " << label << "label from " << id.value() << ".";
            result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
        }
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

Core::ModelRequestResult TaskManager::RemoveAllLabels(const Core::TaskID &id) {
    Core::ModelRequestResult result;
    std::lock_guard lk{mtx_};
    auto it = tasks_.find(id);

    if (it != tasks_.end()) {
        it->second.first.clear_labels();
        LOG_STREAM(debug) << "Removed all labels of " << id.value() << ".";
        result.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    } else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
    }

    return result;
}

void TaskManager::Replace(const std::vector<Core::TaskEntity> &vec) {
    std::lock_guard lk{mtx_};

    tasks_.clear();
    LOG_STREAM(debug) << "Cleared all tasks.";

    Core::TaskID max_id;
    max_id.set_value(0);
    for (const auto &te : vec) {
        Node n;
        if (te.has_parent())
            n.SetParent(te.parent());
        tasks_.insert(std::make_pair(te.id(),
                                    std::make_pair(te.data(), n)));
        if (max_id < te.id())
            max_id = te.id();
    }
    LOG_STREAM(debug) << "Inserted " << tasks_.size() << " tasks.";

    // record children
    for (const auto &kv : tasks_)
        if (kv.second.second.parent())
            tasks_[*kv.second.second.parent()].second.AddChild(kv.first);

    // substitute new IDGenerator with the new state
    gen_ = std::make_shared<IDGenerator>(max_id.value()+1);
    LOG_STREAM(debug) << "Generator state set to " << gen_->state() << ".";
}

std::shared_ptr<IDGenerator> TaskManager::gen() const {
    return gen_;
}
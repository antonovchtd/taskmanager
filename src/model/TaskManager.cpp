//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator) : gen_(generator) {
}

void TaskManager::AddChild(const Core::TaskID &parent_id, const Core::TaskID &child_id) {
    auto it = tasks_.find(parent_id);
    if (it != tasks_.end())
        it->second.second.AddChild(child_id);
}

void TaskManager::RemoveChild(const Core::TaskID &parent_id, const Core::TaskID &child_id) {
    auto it = tasks_.find(parent_id);
    if (it != tasks_.end())
        it->second.second.RemoveChild(child_id);
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

ActionResult TaskManager::Add(const Core::Task &t) {
    Core::TaskID id = gen_->genID();
    if (Validate(id))
        return {ActionResult::Status::DUPLICATE_ID, id};

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node())));
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::AddSubtask(const Core::Task &t, const Core::TaskID &parent) {
    Core::TaskID id = gen_->genID();
    if (Validate(id))
        return {ActionResult::Status::DUPLICATE_ID, id};
    if (!Validate(parent))
        return {ActionResult::Status::PARENT_ID_NOT_FOUND, parent};

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node(parent))));
    AddChild(parent, id);
    return {ActionResult::Status::SUCCESS, id};
}

std::vector<Core::TaskEntity> TaskManager::getTasks() const {
    std::vector<Core::TaskEntity> vec;
    for (const auto &kv : tasks_) {
        if (kv.second.second.parent()) {
            vec.emplace_back(Core::createTaskEntity(kv.first, kv.second.first, *kv.second.second.parent()));
        } else {
            vec.emplace_back(Core::createTaskEntity(kv.first, kv.second.first));
        }
    }
    return vec;
}

std::vector<Core::TaskEntity> TaskManager::getTasks(const std::string &label) const {
    std::vector<Core::TaskEntity> tasks;
    for (const auto &kv : tasks_) {
        for (const auto &task_label : kv.second.first.labels()) {
            if (task_label == label) {
                tasks.emplace_back(Core::createTaskEntity(kv.first, kv.second.first));
                break;
            }
        }
    }
    return tasks;
}

std::vector<Core::TaskEntity> TaskManager::getTaskWithSubtasks(const Core::TaskID &id) const {
    std::vector<Core::TaskEntity> tasks;
    std::optional<Core::Task> task = GetTask(id);
    if (task)
        tasks.emplace_back(Core::createTaskEntity(id, *task));
    else
        return tasks;
    // not including parent, but will include children
    for (const auto &ch_id : ChildrenOf(id)) {
        auto ch_tasks = getTaskWithSubtasks(ch_id);
        for (auto &ch_task : ch_tasks) {
            ch_task.mutable_parent()->CopyFrom(*ParentOf(ch_task.id()));
        }
        tasks.insert(tasks.end(), ch_tasks.begin(), ch_tasks.end());
    }
    return tasks;
}

ActionResult TaskManager::Delete(const Core::TaskID &id, bool deleteChildren) {
    if (Validate(id)) {
        if (!ChildrenOf(id).empty() && !deleteChildren)
            return {ActionResult::Status::HAS_CHILDREN, id};
        std::optional<Core::TaskID> ancestor = ParentOf(id);
        if (ancestor && Validate(*ancestor))
            RemoveChild(*ancestor, id);
        for (auto const &ch: ChildrenOf(id))
            Delete(ch, true);
        tasks_.erase(id);
        return {ActionResult::Status::SUCCESS, id};
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
}

ActionResult TaskManager::Edit(const Core::TaskID &id, const Core::Task &t) {
    auto it = tasks_.find(id);
    if (it != tasks_.end()) {
        it->second.first = t;
        return {ActionResult::Status::SUCCESS, id};
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
}

ActionResult TaskManager::Complete(const Core::TaskID &id) {
    auto it = tasks_.find(id);
    if (it != tasks_.end()) {
        it->second.first.set_is_complete(true);
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }

    for (auto const &ch : ChildrenOf(id))
        Complete(ch);
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::Uncomplete(const Core::TaskID &id) {
    auto it = tasks_.find(id);
    if (it != tasks_.end()) {
        it->second.first.set_is_complete(false);
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    for (auto const &ch : ChildrenOf(id))
        Uncomplete(ch);
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::Validate(const Core::TaskID &id) const{
    if (tasks_.find(id) != tasks_.end())
        return {ActionResult::Status::SUCCESS, id};
    else
        return {ActionResult::Status::ID_NOT_FOUND, id};
}

size_t TaskManager::size() const {
    return tasks_.size();
}

ActionResult TaskManager::AddLabel(const Core::TaskID &id, const std::string &label) {
    auto it = tasks_.find(id);
    if (it != tasks_.end()) {
        auto labels = it->second.first.labels();
        if (find(labels.begin(), labels.end(), label) == labels.end())
            it->second.first.add_labels(label);
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::ClearLabel(const Core::TaskID &id, const std::string &label) {
    auto it = tasks_.find(id);
    if (it != tasks_.end()) {
        auto labels = it->second.first.mutable_labels();
        auto label_it = find(labels->begin(), labels->end(), label);
        if (label_it != labels->end())
            it->second.first.mutable_labels()->erase(label_it);
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::ClearLabels(const Core::TaskID &id) {
    auto it = tasks_.find(id);
    if (it != tasks_.end()) {
        it->second.first.clear_labels();
    } else {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    return {ActionResult::Status::SUCCESS, id};
}

void TaskManager::Replace(const std::vector<Core::TaskEntity> &vec) {
    tasks_.clear();
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
    // record children
    for (const auto &kv : tasks_)
        if (kv.second.second.parent())
            tasks_[*kv.second.second.parent()].second.AddChild(kv.first);

    // substitute new IDGenerator with the new state
    gen_ = std::make_shared<IDGenerator>(max_id.value()+1);
}

std::shared_ptr<IDGenerator> TaskManager::gen() const {
    return gen_;
}
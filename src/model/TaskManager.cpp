//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator) : gen_(generator) {
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
    tasks_.at(parent).second.AddChild(id);
    return {ActionResult::Status::SUCCESS, id};
}

std::vector<Core::TaskEntity> TaskManager::getTasks() const {
    std::vector<Core::TaskEntity> vec;
    for (const auto &kv : tasks_) {
        Core::TaskEntity te;
        te.set_allocated_id(new Core::TaskID(kv.first));
        te.set_allocated_data(new Core::Task(kv.second.first));
        if (kv.second.second.parent()) {
            te.set_allocated_parent(new Core::TaskID(*kv.second.second.parent()));
        }
        vec.push_back(te);
    }
    return vec;
}

std::vector<Core::TaskEntity> TaskManager::getTasks(const std::string &label) const {
    std::vector<Core::TaskEntity> tasks;
    for (const auto &kv : tasks_) {
        for (const auto &task_label : kv.second.first.labels()) {
            if (task_label == label) {
                Core::TaskEntity te;
                te.set_allocated_id(new Core::TaskID(kv.first));
                te.set_allocated_data(new Core::Task(kv.second.first));
                tasks.push_back(te);
                break;
            }
        }
    }
    return tasks;
}

std::vector<Core::TaskEntity> TaskManager::getTasks(const Core::TaskID &id) const {
    std::vector<Core::TaskEntity> tasks;
    Core::TaskEntity te;
    te.set_allocated_id(new Core::TaskID(id));
    te.set_allocated_data(new Core::Task(tasks_.at(id).first));
    // not including parent, but will include children
    tasks.push_back(te);
    for (const auto &ch_id : tasks_.at(id).second.children()) {
        auto ch_tasks = getTasks(ch_id);
        for (auto &ch_task : ch_tasks) {
            ch_task.set_allocated_parent(new Core::TaskID(*tasks_.at(ch_task.id()).second.parent()));
        }
        tasks.insert(tasks.end(), ch_tasks.begin(), ch_tasks.end());
    }
    return tasks;
}

ActionResult TaskManager::Delete(const Core::TaskID &id, bool deleteChildren) {
    try {
        if (!tasks_.at(id).second.children().empty() && !deleteChildren)
            return {ActionResult::Status::HAS_CHILDREN, id};
        std::optional<Core::TaskID> ancestor = tasks_.at(id).second.parent();
        if (ancestor && Validate(ancestor.value()))
            tasks_.at(ancestor.value()).second.RemoveChild(id);
        for (auto const &ch: tasks_.at(id).second.children())
            Delete(ch, true);
        tasks_.erase(id);
        return {ActionResult::Status::SUCCESS, id};
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
}

ActionResult TaskManager::Edit(const Core::TaskID &id, const Core::Task &t) {
    try {
        tasks_.at(id).first = t;
        return {ActionResult::Status::SUCCESS, id};
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
}

ActionResult TaskManager::Complete(const Core::TaskID &id) {
    try {
        tasks_.at(id).first.set_is_complete(true);
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    for (auto const &ch : tasks_.at(id).second.children())
        Complete(ch);
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::Uncomplete(const Core::TaskID &id) {
    try {
        tasks_.at(id).first.set_is_complete(false);
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    for (auto const &ch : tasks_.at(id).second.children())
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
    try {
        auto labels = tasks_.at(id).first.labels();
        if (find(labels.begin(), labels.end(), label) == labels.end())
            tasks_.at(id).first.add_labels(label);
    } catch (const std::out_of_range &) {
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
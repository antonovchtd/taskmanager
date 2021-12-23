//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator) : gen_(generator) {
}
TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator,
                         const Container &tasks) :
                         gen_(generator), tasks_(tasks) {
}

ActionResult TaskManager::Add(const ProtoTask::Task &t) {
    ProtoTask::TaskID id = gen_->genID();
    if (Validate(id))
        return {ActionResult::Status::DUPLICATE_ID, id};

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node())));
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::AddSubtask(const ProtoTask::Task &t, const ProtoTask::TaskID &parent) {
    ProtoTask::TaskID id = gen_->genID();
    if (Validate(id))
        return {ActionResult::Status::DUPLICATE_ID, id};
    if (!Validate(parent))
        return {ActionResult::Status::PARENT_ID_NOT_FOUND, parent};

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node(parent))));
    tasks_.at(parent).second.AddChild(id);
    return {ActionResult::Status::SUCCESS, id};
}

Container TaskManager::getTasks() const {
    return tasks_;
}

Container TaskManager::getTasks(const std::string &label) const {
    decltype(tasks_) tasks;
    for (const auto &kv : tasks_) {
        if (kv.second.first.label() == label) {
            tasks.insert(kv);
            tasks.at(kv.first).second.RemoveParent();
            tasks.at(kv.first).second.RemoveChildren();
        }
    }
    return tasks;
}

Container TaskManager::getTasks(const ProtoTask::TaskID &id) {
    Container tasks;
    tasks.insert(std::make_pair(id, tasks_.at(id)));
    tasks.at(id).second.RemoveParent();
    for (const auto &ch : tasks.at(id).second.children()) {
        auto ch_tasks = getTasks(ch);
        for (auto it = ch_tasks.begin(); it != ch_tasks.end(); it++) {
            it->second.second.SetParent(*tasks_.at(ch).second.parent());
        }
        tasks.insert(ch_tasks.begin(), ch_tasks.end());
    }
    return tasks;
}

ActionResult TaskManager::Delete(const ProtoTask::TaskID &id, bool deleteChildren) {
    try {
        if (!tasks_.at(id).second.children().empty() && !deleteChildren)
            return {ActionResult::Status::HAS_CHILDREN, id};
        std::optional<ProtoTask::TaskID> ancestor = tasks_.at(id).second.parent();
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

ActionResult TaskManager::Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t) {
    try {
        tasks_.at(id).first = t;
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult TaskManager::Complete(const ProtoTask::TaskID &id) {
    return SetComplete(id, true);
}

ActionResult TaskManager::Uncomplete(const ProtoTask::TaskID &id) {
    return SetComplete(id, false);
}

ActionResult TaskManager::SetComplete(const ProtoTask::TaskID &id, bool flag) {
    try {
        tasks_.at(id).first.set_is_complete(flag);
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    for (auto const &ch : tasks_.at(id).second.children())
        SetComplete(ch, flag);
    return {ActionResult::Status::SUCCESS, id};
}

std::pair<ProtoTask::Task, Node>& TaskManager::operator[](const ProtoTask::TaskID &id) {
    return tasks_.at(id);
}

ActionResult TaskManager::Validate(const ProtoTask::TaskID &id) const{
    if (tasks_.find(id) != tasks_.end())
        return {ActionResult::Status::SUCCESS, id};
    else
        return {ActionResult::Status::ID_NOT_FOUND, id};
}

size_t TaskManager::size() const {
    return tasks_.size();
}

ActionResult TaskManager::SetLabel(const ProtoTask::TaskID &id, const std::string &label) {
    ProtoTask::Task t;
    try {
        tasks_.at(id).first.set_label(label);
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, id};
    }
    return {ActionResult::Status::SUCCESS, id};
}

std::vector<ProtoTask::TaskEntity> TaskManager::Export() {
    std::vector<ProtoTask::TaskEntity> vec;
    for (const auto &kv : tasks_) {
        ProtoTask::TaskEntity te;
        te.set_allocated_id(new ProtoTask::TaskID(kv.first));
        te.set_allocated_data(new ProtoTask::Task(kv.second.first));
        if (kv.second.second.parent()) {
            te.set_allocated_parent(new ProtoTask::TaskID(*kv.second.second.parent()));
        }
        vec.push_back(te);
    }
    return vec;
}

void TaskManager::Replace(const std::vector<ProtoTask::TaskEntity> &vec) {
    tasks_.clear();
    ProtoTask::TaskID max_id;
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

    // set IDGenerator state
    gen_->setState(max_id.value()+1);
}

std::shared_ptr<IDGenerator> TaskManager::gen() const {
    return gen_;
}
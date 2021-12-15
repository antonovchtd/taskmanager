//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator) : gen_(generator) {
}
TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator,
                         const std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> &tasks) :
                         gen_(generator), tasks_(tasks) {
}

ProtoTask::TaskID TaskManager::Add(const ProtoTask::Task &t) {
    ProtoTask::TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::Add attempts to overwrite ProtoTask::Task");

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node())));
    return id;
}

ProtoTask::TaskID TaskManager::AddSubtask(const ProtoTask::Task &t, const ProtoTask::TaskID &parent) {
    ProtoTask::TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::AddSubtask attempts to overwrite ProtoTask::Task");
    if (!Validate(parent))
        throw std::runtime_error("TaskManager::AddSubtask invalid parent ID");

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node(parent))));
    tasks_.at(parent).second.AddChild(id);
    return id;
}

std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> TaskManager::getTasks() const {
    return tasks_;
}

std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> TaskManager::getTasks(const std::string &label) const {
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

std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> TaskManager::getTasks(const ProtoTask::TaskID &id) {
    decltype(tasks_) tasks;
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

void TaskManager::Delete(const ProtoTask::TaskID &id, bool deleteChildren) {
    if (!tasks_.at(id).second.children().empty() && !deleteChildren)
        throw std::runtime_error("TaskManager::Delete attempts to delete ProtoTask::Task with subtasks");
    std::optional<ProtoTask::TaskID> ancestor = tasks_.at(id).second.parent();
    if (ancestor && Validate(ancestor.value()))
        tasks_.at(ancestor.value()).second.RemoveChild(id);
    for (auto const &ch : tasks_.at(id).second.children())
        Delete(ch, true);
    tasks_.erase(id);
}

void TaskManager::Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t) {
    tasks_.at(id).first = t;
}

void TaskManager::Complete(const ProtoTask::TaskID &id) {
    ProtoTask::Task t = tasks_.at(id).first;
    t.set_is_complete(true);
    Edit(id, t);
    for (auto const &ch : tasks_.at(id).second.children())
        Complete(ch);
}

std::pair<ProtoTask::Task, Node>& TaskManager::operator[](ProtoTask::TaskID id) {
    return tasks_.at(id);
}

bool TaskManager::Validate(const ProtoTask::TaskID &id) const{
    return tasks_.find(id) != tasks_.end();
}

size_t TaskManager::size() const {
    return tasks_.size();
}

void TaskManager::SetLabel(const ProtoTask::TaskID &id, const std::string &label) {
    ProtoTask::Task t = tasks_.at(id).first;
    t.set_label(label);
    Edit(id, t);
}

void TaskManager::load(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        *this = Persistor::load(file);
        file.close();
    }
    else
        throw std::runtime_error("Could not open file " + filename + ".");
}

void TaskManager::save(const std::string &filename) {
    std::ofstream file(filename, std::ios::trunc | std::ios::binary);
    if (file.is_open()) {
        Persistor::save(*this, file);
        file.close();
    }
    else
        throw std::runtime_error("Could not open file " + filename + ".");
}

std::shared_ptr<IDGenerator> TaskManager::gen() const {
    return gen_;
}
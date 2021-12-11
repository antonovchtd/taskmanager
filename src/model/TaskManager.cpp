//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(const std::shared_ptr<IDGenerator> &generator) : gen_(generator) {
}

TaskID TaskManager::Add(const Task &t) {
    TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::Add attempts to overwrite task");

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node())));
    return id;
}

TaskID TaskManager::AddSubtask(const Task &t, const TaskID &parent) {
    TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::AddSubtask attempts to overwrite task");
    if (parent.isValid() && !Validate(parent))
        throw std::runtime_error("TaskManager::AddSubtask invalid parent ID");

    tasks_.insert(std::make_pair(id, std::make_pair(t, Node(parent))));
    tasks_.at(parent).second.AddChild(id);
    return id;
}

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks() const {
    return tasks_;
}

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks(const std::string &label) const {
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

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks(const TaskID &id) {
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

void TaskManager::Delete(const TaskID &id, bool deleteChildren) {
    if (!tasks_.at(id).second.children().empty() && !deleteChildren)
        throw std::runtime_error("TaskManager::Delete attempts to delete task with subtasks");
    std::optional<TaskID> ancestor = tasks_.at(id).second.parent();
    if (ancestor && Validate(ancestor.value()))
        tasks_.at(ancestor.value()).second.RemoveChild(id);
    for (auto const &ch : tasks_.at(id).second.children())
        Delete(ch, true);
    tasks_.erase(id);
}

void TaskManager::Edit(const TaskID &id, const Task &t) {
    tasks_.at(id).first = t;
}

void TaskManager::Complete(const TaskID &id) {
    Task::Data data = tasks_.at(id).first.data();
    data.is_complete = true;
    Edit(id, Task::Create(data));
    for (auto const &ch : tasks_.at(id).second.children())
        Complete(ch);
}

std::pair<Task, Node>& TaskManager::operator[](TaskID id) {
    return tasks_.at(id);
}

bool TaskManager::Validate(const TaskID &id) const{
    return tasks_.find(id) != tasks_.end();
}

size_t TaskManager::size() const {
    return tasks_.size();
}

void TaskManager::SetLabel(const TaskID &id, const std::string &label) {
    Task::Data data = tasks_.at(id).first.data();
    data.label = label;
    Edit(id, Task::Create(data));
}
//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(std::shared_ptr<IDGenerator> generator) : gen_(std::move(generator)) {
}

TaskID TaskManager::Add(const Task &t) {
    TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::Add attempts to overwrite task");

    tasks_[id] = std::make_pair(t, TaskID::invalidID());
    return id;
}

TaskID TaskManager::AddSubtask(const Task &t, const TaskID &parent) {
    TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::AddSubtask attempts to overwrite task");
    if (parent.isValid() && !Validate(parent))
        throw std::runtime_error("TaskManager::AddSubtask invalid parent ID");

    tasks_[id] = std::make_pair(t, Node(parent));
    tasks_[parent].second.AddChild(id);
    return id;
}

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks() const {
    return tasks_;
}

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks(const std::string &label) const {
    decltype(tasks_) tasks;
    for (const auto &kv : tasks_) {
        if (kv.second.second.label() == label) {
            tasks.insert(kv);
            tasks[kv.first].second.SetParent(TaskID::invalidID());
            tasks[kv.first].second.removeChildren();
        }
    }
    return tasks;
}

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks(const TaskID &id) {
    decltype(tasks_) tasks;
    tasks[id] = tasks_[id];
    tasks[id].second.SetParent(TaskID::invalidID());
    for (const auto &ch : tasks[id].second.children()) {
        auto ch_tasks = getTasks(ch);
        for (auto it = ch_tasks.begin(); it != ch_tasks.end(); it++) {
            it->second.second.SetParent(tasks_[ch].second.parent());
        }
        tasks.insert(ch_tasks.begin(), ch_tasks.end());
    }
    return tasks;
}

void TaskManager::Delete(const TaskID &id, bool deleteChildren) {
    if (!tasks_[id].second.children().empty() && !deleteChildren)
        throw std::runtime_error("TaskManager::Delete attempts to delete task with subtasks");
    std::optional<TaskID> ancestor = tasks_[id].second.parent();
    if (ancestor && Validate(ancestor.value()))
        tasks_[ancestor.value()].second.removeChild(id);
    for (auto const &ch : tasks_[id].second.children())
        Delete(ch, true);
    tasks_.erase(id);
}

void TaskManager::Edit(const TaskID &id, Task t) {
    tasks_[id].first = std::move(t);
}

void TaskManager::Complete(const TaskID &id) {
    Task t = tasks_[id].first;
    this->Edit(id, Task::Create(t.title(), t.priority(), t.dueDate(), true));
    for (auto const &ch : tasks_[id].second.children())
        Complete(ch);
}

std::pair<Task, Node>& TaskManager::operator[](TaskID id) {
    return tasks_[id];
}

bool TaskManager::Validate(const TaskID &id) const{
    return tasks_.find(id) != tasks_.end();
}

size_t TaskManager::size() const {
    return tasks_.size();
}

void TaskManager::SetLabel(const TaskID &id, const std::string &label) {
    tasks_[id].second.SetLabel(label);
}
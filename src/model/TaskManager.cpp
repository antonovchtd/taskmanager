//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

TaskManager::TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {
}

TaskManager::TaskManager(std::shared_ptr<IDGenerator> generator) : gen_(std::move(generator)) {
}

TaskID TaskManager::Add(Task t, TaskID ancestor) {
    TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::Add attempts to overwrite task");

    tasks_[id] = std::make_pair(std::move(t), Node(ancestor));
    if (ancestor.isValid())
        tasks_[ancestor].second.AddChild(id);
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
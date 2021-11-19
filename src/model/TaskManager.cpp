//
// Created by Anton Ovcharenko on 08.11.2021.
//
#include <iostream>

#include "TaskManager.h"

TaskID TaskManager::Add(Task t, TaskID ancestor) {
    TaskID id = gen_->genID();
    if (Validate(id))
        throw std::runtime_error("TaskManager::Add attempts to overwrite task");
    tasks_[id] = std::make_pair(std::move(t), Node(ancestor));
    if (ancestor.value())
        tasks_[ancestor].second.AddChild(id);
    return id;
}

std::map<TaskID, std::pair<Task, Node>> TaskManager::getTasks() const {
    return tasks_;
}

void TaskManager::Delete(TaskID k) {
    tasks_.erase(k);
}

void TaskManager::Edit(TaskID id, Task t) {
    tasks_[id].first = std::move(t);
}

void TaskManager::Complete(TaskID id) {
    Task t = tasks_[id].first;
    this->Edit(id, Task::Create(t.title(), t.priority(), t.due_date(), true));
}

std::pair<Task, Node> TaskManager::operator[](TaskID id) {
    return tasks_[id];
}

std::ostream & operator<<(std::ostream &os, const TaskID& id);
std::ostream & operator<<(std::ostream &os, const Task& t);

void TaskManager::recursivePrint(std::ostream &os, TaskID id, std::string prefix) const {
    auto value = tasks_.at(id);
    os << prefix << id << " — " << value.first;
    for (const auto &ch : value.second.children())
        recursivePrint(os, ch, prefix + "    ");
}

std::ostream & operator<<(std::ostream &os, const TaskManager& tm){
    auto tasks = tm.getTasks();
    for (const auto &t : tasks){
        TaskID id = t.first;
        auto value = t.second;
        if (value.second.parent() == TaskID(0))
            tm.recursivePrint(os, id, "");
    }
    return os;
}

void TaskManager::Show(std::ostream &os) const{
    os << *this;
}

bool TaskManager::Validate(TaskID id) const{
    return tasks_.find(id) != tasks_.end();
}

size_t TaskManager::size() const {
    return tasks_.size();
}
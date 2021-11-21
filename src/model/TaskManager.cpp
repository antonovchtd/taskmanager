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

void TaskManager::Delete(TaskID id) {
    if (!tasks_[id].second.children().empty())
        throw std::runtime_error("TaskManager::Delete attempts to delete task with subtasks");
    TaskID ancestor = tasks_[id].second.parent();
    if (Validate(ancestor))
        tasks_[ancestor].second.removeChild(id);
    tasks_.erase(id);
}

void TaskManager::Edit(TaskID id, Task t) {
    tasks_[id].first = std::move(t);
}

void TaskManager::Complete(TaskID id) {
    Task t = tasks_[id].first;
    this->Edit(id, Task::Create(t.title(), t.priority(), t.due_date(), true));
}

std::pair<Task, Node>& TaskManager::operator[](TaskID id) {
    return tasks_[id];
}

std::ostream & operator<<(std::ostream &os, const TaskID& id);
std::ostream & operator<<(std::ostream &os, const Task& t);

std::ostream & operator<<(std::ostream &os, const std::pair<TaskID, std::pair<Task, Node>>& kv){
    os << kv.first << " — " << kv.second.first;
    return os;
}

void TaskManager::recursivePrint(std::ostream &os, const std::pair<TaskID, std::pair<Task, Node>>& kv, const std::string& prefix) const {
    os << prefix << kv;
    for (const auto &id : kv.second.second.children()) {
        auto ch = std::make_pair(id, tasks_.at(id));
        recursivePrint(os, ch, prefix + "    ");
    }
}

void TaskManager::Show(std::ostream &os) const{
    for (const auto &kv : tasks_){
        if (kv.second.second.parent() == TaskID::invalidID())
            recursivePrint(os, kv, "");
    }
}

void TaskManager::Show(std::ostream &os, std::string &label) const{
    for (const auto &kv : tasks_){
        if (kv.second.second.label() == label)
            os << kv;
    }
}

bool TaskManager::Validate(TaskID id) const{
    return tasks_.find(id) != tasks_.end();
}

size_t TaskManager::size() const {
    return tasks_.size();
}
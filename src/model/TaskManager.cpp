//
// Created by Anton Ovcharenko on 08.11.2021.
//
#include <iostream>

#include "TaskManager.h"

TaskID TaskManager::Add(Task t) {
    TaskID id = gen_.genID();
    tasks_[id] = std::move(t);
    return id;
}

std::map<TaskID, Task> TaskManager::getTasks() {
    return tasks_;
}

void TaskManager::Delete(TaskID k) {
    tasks_.erase(k);
}

void TaskManager::Edit(TaskID k, Task t) {
    tasks_[k] = std::move(t);
}

void TaskManager::Complete(TaskID k) {
    tasks_[k].setComplete(true);
}

std::ostream & operator<<(std::ostream &os, TaskManager tm){
    auto tasks = tm.getTasks();
    const std::string priorityName[] = {"High", "Medium", "Low", "None"};
    for (const auto &t : tasks){
        os << t.first.value() << " — " << t.second.title() << ", Priority: " <<
           priorityName[static_cast<int>(t.second.priority())];
        if (t.second.due_date() < time(nullptr))
            os << " [overdue] ";
        else {
            time_t dd = t.second.due_date();
            os << ", Due: " << asctime(localtime(&dd));
        }
        if (t.second.isComplete()){
            os << " [completed] ";
        }
        os << std::endl;

    }
    return os;
}

void TaskManager::Show(std::ostream &os) {
    os << *this;
}

bool TaskManager::Validate(TaskID id) const{
    return tasks_.find(id) != tasks_.end();
}

bool TaskID::operator==(const TaskID& rhs) const{
    return value_ == rhs.value();
}

bool TaskID::operator<(const TaskID& rhs) const{
    return value_ < rhs.value();
}
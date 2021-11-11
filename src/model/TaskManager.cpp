//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include <iostream>

#include "TaskManager.h"

int TaskManager::Add(Task t) {
    tasks_[counter_] = std::move(t);
    return counter_++;
}

std::map<int, Task> TaskManager::getTasks() {
    return tasks_;
}

void TaskManager::Delete(int k) {
    tasks_.erase(k);
}

void TaskManager::Edit(int k, Task t) {
    tasks_[k] = std::move(t);
}

void TaskManager::Complete(int k) {
    tasks_[k].setComplete(true);
}

void TaskManager::Show() {

}

std::ostream & operator<<(std::ostream &os, std::map<int, Task>& tasks){
    const std::string priorityName[] = {"High", "Medium", "Low", "None"};
    for (const auto &t : tasks){
        os << t.first << " — " << t.second.title_ << ", Priority: " <<
            priorityName[static_cast<int>(t.second.priority_)];
        if (t.second.due_date_ < time(nullptr))
            os << " [overdue] ";
        else
            os << ", Due: " << asctime(localtime(&t.second.due_date_));
        if (t.second.isComplete_){
            os << " [completed] ";
        }
        os << std::endl;

    }
    return os;
}
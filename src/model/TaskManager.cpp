//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "TaskManager.h"

unsigned int TaskManager::Add(Task t) {
    TaskID id = gen_.genID();
    tasks_[id.value()] = std::move(t);
    return id.value();
}

std::map<unsigned int, Task> TaskManager::getTasks() {
    return tasks_;
}

void TaskManager::Delete(unsigned int k) {
    tasks_.erase(k);
}

void TaskManager::Edit(unsigned int k, Task t) {
    tasks_[k] = std::move(t);
}

void TaskManager::Complete(unsigned int k) {
    tasks_[k].setComplete(true);
}

void TaskManager::Show() {

}

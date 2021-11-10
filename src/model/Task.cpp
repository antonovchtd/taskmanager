//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "Task.h"

Task Task::Create(const std::string &title, Task::Priority p, time_t due_date) {
    return {title, p, due_date};
}

Task Task::Create(const std::string &title, Task::Priority p) {
    return {title, p, 0};
}

Task Task::Create(const std::string &title, time_t due_date) {
    return {title, Task::Priority::NONE, due_date};
}

void Task::setComplete(bool status) {
    isComplete_ = status;
}

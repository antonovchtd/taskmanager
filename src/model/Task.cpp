//
// Created by Anton Ovcharenko on 08.11.2021.
//
#include <iostream>

#include "Task.h"

Task::Task(std::string title, Task::Priority p, time_t due_date, bool complete_flag) :
    title_{std::move(title)},
    priority_{p},
    due_date_{due_date},
    isComplete_(complete_flag)
{
};

Task Task::Create(const Task::Data &d) {
    return Task::Create(d.title_, d.priority_, d.due_date_, d.isComplete_);
}

Task Task::Create(const std::string &title, Task::Priority p, time_t due_date, bool complete_flag) {
    if (title.empty())
        throw std::runtime_error("Empty title string.");
    return {title, p, due_date, complete_flag};
}

bool Task::operator==(const Task& rhs) const{
    return title_ == rhs.title()
        && priority_ == rhs.priority()
        && due_date_ == rhs.due_date();
}

std::ostream & operator<<(std::ostream &os, const Task& t) {
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
    if (t.due_date() < time(nullptr))
        os << " [overdue] ";
    else {
        time_t dd = t.due_date();
        std::string str_time = std::string(asctime(localtime(&dd)));
        str_time.pop_back();
        os << ", Due: " << str_time;
    }
    if (t.isComplete()) {
        os << " [completed] ";
    }
    os << std::endl;
    return os;
}

Task::Data Task::data() const {
    return Task::Data{title(), priority(), due_date(), isComplete()};
}
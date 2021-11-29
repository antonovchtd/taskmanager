//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "Task.h"

Task::Task(std::string title, Task::Priority p, time_t due_date, bool complete_flag) :
        title_{std::move(title)},
        priority_{p},
        due_date_{due_date},
        is_complete_(complete_flag)
{
}

Task Task::Create(const Task::Data &d) {
    return Task::Create(d.title, d.priority, d.due_date, d.is_complete);
}

Task Task::Create(const std::string &title, Task::Priority p, time_t due_date, bool complete_flag) {
    if (title.empty())
        throw std::runtime_error("Empty title string.");
    return {title, p, due_date, complete_flag};
}

bool Task::operator==(const Task& rhs) const {
    return title_ == rhs.title()
        && priority_ == rhs.priority()
        && due_date_ == rhs.dueDate();
}

std::ostream & operator<<(std::ostream &os, const Task& t) {
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
    if (t.dueDate() < time(nullptr)) {
        os << " [overdue] ";
    } else {
        time_t dd = t.dueDate();
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
    return Task::Data{title_, priority_, due_date_, is_complete_};
}
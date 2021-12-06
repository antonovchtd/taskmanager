//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "Task.h"

Task::Task() : Task("Default", Priority::NONE, 0, false) {
}

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

time_t Task::dueDate() const {
    return due_date_;
}

std::string Task::title() const {
    return title_;
}

Task::Priority Task::priority() const {
    return priority_;
}

bool Task::isComplete() const {
    return is_complete_;
}

bool Task::operator==(const Task& rhs) const {
    return title_ == rhs.title()
        && priority_ == rhs.priority()
        && due_date_ == rhs.dueDate();
}

Task::Data Task::data() const {
    return Task::Data{title_, priority_, due_date_, is_complete_};
}

std::string Task::to_string() const {
    std::ostringstream os;
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << title_ << ", Priority: " <<
       priorityName[static_cast<int>(priority_)];
    time_t dd = dueDate();
    std::string str_time = std::string(asctime(localtime(&dd)));
    str_time.pop_back();
    os << ", Due: " << str_time;
    if (due_date_ < time(nullptr))
        os << " [overdue] ";

    if (is_complete_) {
        os << " [completed] ";
    }
    return os.str();
}
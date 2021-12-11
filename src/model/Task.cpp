//
// Created by Anton Ovcharenko on 08.11.2021.
//

#include "Task.h"

Task::Task(const std::string &title, Task::Priority p, time_t due_date,
           const std::string &label, bool complete_flag) :
        title_{title},
        priority_{p},
        due_date_{due_date},
        label_{label},
        is_complete_(complete_flag)
{
}

Task Task::Create(const Task::Data &d) {
    return Task::Create(d.title, d.priority, d.due_date, d.label, d.is_complete);
}

Task Task::Create(const std::string &title, Task::Priority p, time_t due_date,
                  const std::string &label, bool complete_flag) {
    if (title.empty())
        throw std::runtime_error("Empty title string.");
    return {title, p, due_date, label, complete_flag};
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

std::string Task::label() const {
    return label_;
}

bool Task::operator==(const Task& rhs) const {
    return title_ == rhs.title()
        && priority_ == rhs.priority()
        && label_ == rhs.label()
        && due_date_ == rhs.dueDate();
}

Task::Data Task::data() const {
    return Task::Data{title_, priority_, due_date_, label_,is_complete_};
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
        os << " [overdue]";

    if (is_complete_) {
        os << " [completed]";
    }

    if (!label_.empty()) {
        os << " L: " << label_;
    }

    return os.str();
}
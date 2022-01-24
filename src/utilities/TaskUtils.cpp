//
// Created by Anton Ovcharenko on 23.12.2021.
//

#include "TaskUtils.h"

bool Core::operator==(const Core::Task &lhs, const Core::Task &rhs) {
    return rhs.title() == lhs.title() &&
           rhs.priority() == lhs.priority() &&
           rhs.is_complete() == lhs.is_complete() &&
           rhs.labels() == lhs.labels() &&
           rhs.due_date() == lhs.due_date();
}

bool Core::operator==(const repStr &lhs, const repStr &rhs) {
    if (lhs.size() != rhs.size())
        return false;

    for (int i = 0; i < lhs.size(); ++i) {
        if (lhs.at(i) != rhs.at(i))
            return false;
    }

    return true;

}

std::string to_string(const Core::Task &t) {
    std::ostringstream os;
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
//    time_t dd = t.due_date().seconds();
    time_t dd = t.due_date();
    std::string str_time = std::string(asctime(localtime(&dd)));
    str_time.pop_back();
    os << ", Due: " << str_time;

    if (!t.labels().empty()) {
        os << ", has " << t.labels().size() << " label(s)";
    }

    if (dd < time(nullptr)) {
        os << " [overdue]";
    }

    if (t.is_complete()) {
        os << " [completed]";
    }

    return os.str();
}

Core::Task Core::createTask(const std::string &title,
                            const Core::Task::Priority &priority,
                            const time_t &due_date,
                            const std::string &label,
                            bool is_complete) {
    Core::Task t;
    t.set_title(title);
    t.set_priority(priority);
    t.set_due_date(due_date);
    t.add_labels(label);
    t.set_is_complete(is_complete);
    return t;
}

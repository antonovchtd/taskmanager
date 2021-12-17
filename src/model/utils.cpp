//
// Created by Anton Ovcharenko on 14.12.2021.
//

#include "utils.h"
#include "Node.h"

bool ProtoTask::operator==(const ProtoTask::Task &lhs, const ProtoTask::Task &rhs) {
    return rhs.title() == lhs.title() &&
           rhs.priority() == lhs.priority() &&
           rhs.is_complete() == lhs.is_complete() &&
           rhs.label() == lhs.label() &&
           rhs.due_date() == lhs.due_date();
}
bool ProtoTask::operator==(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    if (rhs.has_num() && lhs.has_num())
        return rhs.num() == lhs.num();
    return false;
}

bool ProtoTask::operator!=(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    return !(lhs == rhs);
}

bool ProtoTask::operator<(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    if (lhs.has_num() && rhs.has_num())
        return lhs.num() < rhs.num();
    return false;
}
bool ProtoTask::operator>(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    if (lhs.has_num() && rhs.has_num())
        return lhs.num() > rhs.num();
    return false;
}

std::string to_string(const ProtoTask::Task &t) {
    std::ostringstream os;
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
//    time_t dd = t.due_date().seconds();
    time_t dd = t.due_date();
    std::string str_time = std::string(asctime(localtime(&dd)));
    str_time.pop_back();
    os << ", Due: " << str_time;
    if (dd < time(nullptr))
        os << " [overdue]";

    if (t.is_complete()) {
        os << " [completed]";
    }

    if (!t.label().empty()) {
        os << " L: " << t.label();
    }

    return os.str();
}

ProtoTask::Task ProtoTask::create(const std::string &title,
                       const ProtoTask::Task::Priority &priority,
                       const time_t &due_date,
                       const std::string &label,
                       bool is_complete) {
    ProtoTask::Task t;
    t.set_title(title);
    t.set_priority(priority);
    t.set_due_date(due_date);
    t.set_label(label);
    t.set_is_complete(is_complete);
    return t;
}

bool operator==(const Node &lhs, const Node &rhs) {
    if (rhs.children() != lhs.children())
        return false;
    else {
        if (lhs.parent().has_value() && rhs.parent().has_value()) {
            return *lhs.parent() == *rhs.parent();
        } else if (!(!lhs.parent().has_value() && !rhs.parent().has_value())) {
            return false;
        }
    }
    return true;
}
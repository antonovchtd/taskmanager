//
// Created by Anton Ovcharenko on 14.12.2021.
//

#include "utils.h"

bool operator==(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    if (rhs.has_num() && lhs.has_num())
        return rhs.num() == lhs.num();
    return false;
}

bool operator!=(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    return !(lhs == rhs);
}

bool operator<(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    if (lhs.has_num() && rhs.has_num())
        return lhs.num() < rhs.num();
    return false;
}
bool operator>(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    if (lhs.has_num() && rhs.has_num())
        return lhs.num() > rhs.num();
    return false;
}

std::string to_string(const ProtoTask::Task &t) {
    std::ostringstream os;
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
    time_t dd = t.due_date().seconds();
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
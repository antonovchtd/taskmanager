//
// Created by Anton O. on 11/16/21.
//

#include "TaskID.h"

unsigned int TaskID::value() const {
    return value_;
}

std::optional<TaskID> TaskID::invalidID() {
    return std::nullopt;
}

bool TaskID::operator==(const TaskID& rhs) const {
    return value_ == rhs.value();
}

std::string TaskID::str() const {
    return std::to_string(value_);
}

bool TaskID::operator!=(const TaskID& rhs) const {
    return !(*this == rhs);
}

bool TaskID::operator<(const TaskID& rhs) const {
    return value_ < rhs.value();
}

std::ostream & operator<<(std::ostream &os, const TaskID& id) {
    os << id.value();
    return os;
}

std::istream& operator>>(std::istream& is, TaskID& id) {
    is >> id.value_;
    return is;
}
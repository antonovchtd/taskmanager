//
// Created by Anton O. on 11/16/21.
//

#include "TaskID.h"

TaskID::TaskID(int val) : value_(val) {
}

TaskID TaskID::Create(const std::string &str) {
    try {
        if (str.empty())
            throw std::invalid_argument{""};
        return TaskID(std::stoi(str));
    }
    catch (const std::invalid_argument &) {
        return TaskID::invalidID();
    }
}

int TaskID::value() const {
    return value_;
}

TaskID TaskID::invalidID() {
    return TaskID(-1);
}

TaskID TaskID::nullid() {
    return TaskID(0);
}

bool TaskID::isValid() const {
    return value_ > 0;
}

bool TaskID::isValidOrNull() const {
    return value_ >= 0;
}

bool TaskID::operator==(const TaskID &rhs) const {
    return value_ == rhs.value();
}

std::string TaskID::to_string() const {
    return std::to_string(value_);
}

bool TaskID::operator!=(const TaskID &rhs) const {
    return !(*this == rhs);
}

bool TaskID::operator<(const TaskID &rhs) const {
    return value_ < rhs.value();
}

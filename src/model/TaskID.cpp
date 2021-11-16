//
// Created by Anton O. on 11/16/21.
//

#include "TaskID.h"

unsigned int TaskID::value() const {
    return value_;
}

bool TaskID::operator==(const TaskID& rhs) const{
    return value_ == rhs.value();
}

bool TaskID::operator<(const TaskID& rhs) const{
    return value_ < rhs.value();
}
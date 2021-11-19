//
// Created by Anton O. on 11/16/21.
//
#include <iostream>

#include "TaskID.h"

unsigned int TaskID::value() const {
    return value_;
}

TaskID TaskID::invalidID() {
    return TaskID(0);
}

bool TaskID::operator==(const TaskID& rhs) const{
    return value_ == rhs.value();
}

bool TaskID::operator!=(const TaskID& rhs) const{
    return !(*this == rhs);
}

bool TaskID::operator<(const TaskID& rhs) const{
    return value_ < rhs.value();
}

std::ostream & operator<<(std::ostream &os, const TaskID& id){
    os << id.value();
    return os;
}
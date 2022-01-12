//
// Created by Anton Ovcharenko on 23.12.2021.
//

#include "TaskIDUtils.h"

bool Core::operator==(const Core::TaskID &lhs, const Core::TaskID &rhs) {
    return rhs.value() == lhs.value();
}

bool Core::operator!=(const Core::TaskID &lhs, const Core::TaskID &rhs) {
    return !(lhs == rhs);
}

bool Core::operator<(const Core::TaskID &lhs, const Core::TaskID &rhs) {
    return lhs.value() < rhs.value();
}
bool Core::operator>(const Core::TaskID &lhs, const Core::TaskID &rhs) {
    return lhs.value() > rhs.value();
}

bool operator==(const std::vector<Core::TaskID> &lhs, const std::vector<Core::TaskID> &rhs) {
    for (const auto &id : lhs) {
        auto it = std::find(rhs.cbegin(), rhs.cend(), id);
        if (it == rhs.cend())
            return false;
    }
    return true;
}

bool operator!=(const std::vector<Core::TaskID> &lhs, const std::vector<Core::TaskID> &rhs) {
    return !(lhs == rhs);
}

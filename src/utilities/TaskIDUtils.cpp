//
// Created by Anton Ovcharenko on 23.12.2021.
//

#include "TaskIDUtils.h"

bool ProtoTask::operator==(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    return rhs.value() == lhs.value();
}

bool ProtoTask::operator!=(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    return !(lhs == rhs);
}

bool ProtoTask::operator<(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    return lhs.value() < rhs.value();
}
bool ProtoTask::operator>(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs) {
    return lhs.value() > rhs.value();
}

bool operator==(const std::vector<ProtoTask::TaskID> &lhs, const std::vector<ProtoTask::TaskID> &rhs) {
    for (const auto &id : lhs) {
        auto it = std::find(rhs.cbegin(), rhs.cend(), id);
        if (it == rhs.cend())
            return false;
    }
    return true;
}

bool operator!=(const std::vector<ProtoTask::TaskID> &lhs, const std::vector<ProtoTask::TaskID> &rhs) {
    return !(lhs == rhs);
}

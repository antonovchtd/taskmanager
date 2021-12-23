//
// Created by Anton Ovcharenko on 23.12.2021.
//

#ifndef TASKMANAGER_SRC_UTILITIES_TASKIDUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_TASKIDUTILS_H_

#include "model/Task.pb.h"

namespace ProtoTask {
    bool operator==(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
    bool operator!=(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
    bool operator<(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
    bool operator>(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
}
bool operator==(const std::vector<ProtoTask::TaskID> &lhs, const std::vector<ProtoTask::TaskID> &rhs);
bool operator!=(const std::vector<ProtoTask::TaskID> &lhs, const std::vector<ProtoTask::TaskID> &rhs);

#endif //TASKMANAGER_SRC_UTILITIES_TASKIDUTILS_H_

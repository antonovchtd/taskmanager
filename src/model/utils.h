//
// Created by Anton Ovcharenko on 14.12.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_UTILS_H_
#define TASKMANAGER_SRC_MODEL_UTILS_H_

#include <sstream>
#include "Task.pb.h"

bool operator==(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
bool operator!=(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
namespace ProtoTask {
    bool operator<(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
}
bool operator>(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
std::string to_string(const ProtoTask::Task &t);

#endif //TASKMANAGER_SRC_MODEL_UTILS_H_

//
// Created by Anton Ovcharenko on 14.12.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_UTILS_H_
#define TASKMANAGER_SRC_MODEL_UTILS_H_

#include <sstream>
#include "Task.pb.h"

class Node;

namespace ProtoTask {
bool operator==(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
bool operator==(const ProtoTask::Task &lhs, const ProtoTask::Task &rhs);
bool operator!=(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
bool operator<(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
bool operator>(const ProtoTask::TaskID &lhs, const ProtoTask::TaskID &rhs);
ProtoTask::Task create(const std::string &title,
                       const ProtoTask::Task::Priority &priority,
                       const time_t &due_date,
                       const std::string &label,
                       bool is_complete);
}
std::string to_string(const ProtoTask::Task &t);
bool operator==(const Node &lhs, const Node &rhs);

#endif //TASKMANAGER_SRC_MODEL_UTILS_H_

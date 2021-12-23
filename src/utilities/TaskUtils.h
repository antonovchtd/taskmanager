//
// Created by Anton Ovcharenko on 23.12.2021.
//

#ifndef TASKMANAGER_SRC_UTILITIES_TASKUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_TASKUTILS_H_

#include <sstream>
#include "Task.pb.h"

namespace ProtoTask {
bool operator==(const ProtoTask::Task &lhs, const ProtoTask::Task &rhs);
ProtoTask::Task createTask(const std::string &title,
                           const ProtoTask::Task::Priority &priority,
                           const time_t &due_date,
                           const std::string &label,
                           bool is_complete);
}
std::string to_string(const ProtoTask::Task &t);

#endif //TASKMANAGER_SRC_UTILITIES_TASKUTILS_H_

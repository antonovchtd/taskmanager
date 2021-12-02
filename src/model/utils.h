//
// Created by Anton O. on 12/1/21.
//

#ifndef TASKMANAGER_SRC_MODEL_UTILS_H_
#define TASKMANAGER_SRC_MODEL_UTILS_H_

#include "Task.h"
#include "TaskID.h"
#include "Node.h"

std::ostream & operator<<(std::ostream &os, const Task& t);
std::ostream & operator<<(std::ostream &os, const TaskID& id);
std::ostream & operator<<(std::ostream &os, const std::pair<TaskID, std::pair<Task, Node>>& kv);
std::ostream & operator<<(std::ostream &os, const std::map<TaskID, std::pair<Task, Node>>& tasks);

namespace TaskUtilsPrinter{
void recursivePrint(std::ostream &os,
                    const std::map<TaskID, std::pair<Task, Node>> &tasks,
                    const std::pair<TaskID, std::pair<Task, Node>> &kv,
                    const std::string &prefix);
}

#endif //TASKMANAGER_SRC_MODEL_UTILS_H_

//
// Created by Anton Ovcharenko on 19.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_NODE_H_
#define TASKMANAGER_SRC_MODEL_NODE_H_

#include <vector>
#include "TaskID.h"

class Node{
public:
    Node() : ancestor_(TaskID(0)) { };
    Node(TaskID ancestor) : ancestor_(ancestor) { };
    void AddChild(const TaskID&);
    TaskID parent();
    std::vector<TaskID> children();

private:
    TaskID ancestor_;
    std::vector<TaskID> children_;

};

#endif //TASKMANAGER_SRC_MODEL_NODE_H_

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
    void removeChild(const TaskID&);
    TaskID parent() const;
    std::string label() const;
    std::vector<TaskID> children() const;
    void SetLabel(const std::string &);

private:
    TaskID ancestor_;
    std::vector<TaskID> children_;
    std::string label_;
};

#endif //TASKMANAGER_SRC_MODEL_NODE_H_

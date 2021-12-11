//
// Created by Anton Ovcharenko on 19.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_NODE_H_
#define TASKMANAGER_SRC_MODEL_NODE_H_

#include <vector>
#include "TaskID.h"

class Node{
public:
    Node();
    Node(const TaskID &parent);

public:
    std::optional<TaskID> parent() const;
    std::vector<TaskID> children() const;

public:
    void SetParent(const TaskID &);
    void RemoveParent();
    void RemoveChild(const TaskID &id);
    void RemoveChildren();
    void AddChild(const TaskID &);

private:
    std::optional<TaskID> parent_;
    std::vector<TaskID> children_;
};

#endif //TASKMANAGER_SRC_MODEL_NODE_H_

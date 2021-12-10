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
    std::string label() const;
    std::vector<TaskID> children() const;

public:
    void SetParent(const TaskID &);
    void RemoveParent();
    void RemoveChild(const TaskID &id);
    void RemoveChildren();
    void AddChild(const TaskID &);
    void SetLabel(const std::string &);

private:
    std::optional<TaskID> parent_;
    std::vector<TaskID> children_;
    std::string label_;
};

#endif //TASKMANAGER_SRC_MODEL_NODE_H_

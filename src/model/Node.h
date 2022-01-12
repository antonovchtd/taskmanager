//
// Created by Anton Ovcharenko on 19.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_NODE_H_
#define TASKMANAGER_SRC_MODEL_NODE_H_

#include <vector>
#include "Task.pb.h"
#include "utilities/TaskIDUtils.h"
#include "utilities/NodeUtils.h"

class Node{
public:
    Node();
    explicit Node(const Core::TaskID &parent);

public:
    std::optional<Core::TaskID> parent() const;
    std::vector<Core::TaskID> children() const;

public:
    void SetParent(const Core::TaskID &);
    void RemoveParent();
    void RemoveChild(const Core::TaskID &id);
    void RemoveChildren();
    void AddChild(const Core::TaskID &);

private:
    std::optional<Core::TaskID> parent_;
    std::vector<Core::TaskID> children_;
};

#endif //TASKMANAGER_SRC_MODEL_NODE_H_

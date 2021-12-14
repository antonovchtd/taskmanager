//
// Created by Anton Ovcharenko on 19.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_NODE_H_
#define TASKMANAGER_SRC_MODEL_NODE_H_

#include <vector>
#include "Task.pb.h"
#include "utils.h"

class Node{
public:
    Node();
    Node(const ProtoTask::TaskID &parent);

public:
    std::optional<ProtoTask::TaskID> parent() const;
    std::vector<ProtoTask::TaskID> children() const;

public:
    void SetParent(const ProtoTask::TaskID &);
    void RemoveParent();
    void RemoveChild(const ProtoTask::TaskID &id);
    void RemoveChildren();
    void AddChild(const ProtoTask::TaskID &);

private:
    std::optional<ProtoTask::TaskID> parent_;
    std::vector<ProtoTask::TaskID> children_;
};

#endif //TASKMANAGER_SRC_MODEL_NODE_H_

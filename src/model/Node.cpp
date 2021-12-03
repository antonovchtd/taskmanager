//
// Created by Anton Ovcharenko on 19.11.2021.
//

#include "Node.h"

Node::Node() : ancestor_(TaskID::invalidID()) {
}

Node::Node(TaskID ancestor) : ancestor_(ancestor) {
}

void Node::AddChild(const TaskID& id) {
    children_.push_back(id);
}

void Node::removeChild(const TaskID& id) {
    std::vector<TaskID>::iterator ch;
    for (ch = children_.begin(); ch != children_.end(); ++ch) {
        if (*ch == id)
            break;
    }
    if (ch != children_.end())
        children_.erase(ch);
}

TaskID Node::parent() const {
    return ancestor_;
}

std::string Node::label() const {
    return label_;
}

std::vector<TaskID> Node::children() const {
    return children_;
}

void Node::SetLabel(const std::string & s) {
    label_ = s;
}
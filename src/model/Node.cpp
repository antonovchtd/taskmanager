//
// Created by Anton Ovcharenko on 19.11.2021.
//

#include "Node.h"

Node::Node() : parent_(std::nullopt) {
}

Node::Node(const TaskID &parent) : parent_(parent) {
}

void Node::AddChild(const TaskID &id) {
    children_.push_back(id);
}

void Node::RemoveChild(const TaskID &id) {
    std::vector<TaskID>::iterator ch;
    for (ch = children_.begin(); ch != children_.end(); ++ch) {
        if (*ch == id)
            break;
    }
    if (ch != children_.end())
        children_.erase(ch);
}

void Node::RemoveChildren() {
    children_.clear();
}

void Node::RemoveParent() {
    parent_ = std::nullopt;
}

std::optional<TaskID> Node::parent() const {
    return parent_;
}

std::string Node::label() const {
    return label_;
}

std::vector<TaskID> Node::children() const {
    return children_;
}

void Node::SetLabel(const std::string &s) {
    label_ = s;
}

void Node::SetParent(const TaskID &id) {
    parent_ = id;
}
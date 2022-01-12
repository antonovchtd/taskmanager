//
// Created by Anton Ovcharenko on 19.11.2021.
//

#include "Node.h"

Node::Node() : parent_(std::nullopt) {
}

Node::Node(const Core::TaskID &parent) : parent_(parent) {
}

void Node::AddChild(const Core::TaskID &id) {
    children_.push_back(id);
}

void Node::RemoveChild(const Core::TaskID &id) {
    std::vector<Core::TaskID>::const_iterator ch;
    for (ch = children_.cbegin(); ch != children_.cend(); ++ch) {
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

std::optional<Core::TaskID> Node::parent() const {
    return parent_;
}

std::vector<Core::TaskID> Node::children() const {
    return children_;
}

void Node::SetParent(const Core::TaskID &id) {
    parent_ = id;
}
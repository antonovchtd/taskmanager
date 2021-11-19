//
// Created by Anton Ovcharenko on 19.11.2021.
//

#include "Node.h"

void Node::AddChild(const TaskID& id){
    children_.push_back(id);
}

void Node::removeChild(const TaskID& id){
    std::vector<TaskID>::iterator ch;
    for (ch = children_.begin(); ch != children_.end(); ++ch){
        if (*ch == id)
            break;
    }
    if (ch != children_.end())
        children_.erase(ch);
}

TaskID Node::parent(){
    return ancestor_;
}

std::vector<TaskID> Node::children(){
    return children_;
}
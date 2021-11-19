//
// Created by Anton Ovcharenko on 19.11.2021.
//

#include "Node.h"

void Node::AddChild(const TaskID& id){
    children_.push_back(id);
}

TaskID Node::parent(){
    return ancestor_;
}

std::vector<TaskID> Node::children(){
    return children_;
}
//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

void Context::execute(){
    state_->execute(*this);
}

void Context::changeState(const std::shared_ptr<State> &s) {
    state_ = s;
}

//void Context::setTask(const Task &t){
//    task_ = t;
//}
//
//Task Context::getTask() const{
//    return task_;
//}

void Context::setTitle(const std::string &title){
    title_ = title;
}
void Context::setDueDate(const std::string & dd){
    due_date_ = dd;
}
void Context::setPriority(const Task::Priority &p){
    priority_ = p;
}
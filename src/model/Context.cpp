//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

void Context::execute(StateFactory &f) {
    state_->execute(*this, f);
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
    data_.title = title;
}
void Context::setDueDate(const time_t & dd){
    data_.due_date = dd;
}
void Context::setPriority(const Task::Priority &p){
    data_.priority = p;
}
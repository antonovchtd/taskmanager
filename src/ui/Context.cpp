//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

void Context::execute(StepFactory &f) {
    step_->execute(*this, f);
}

void Context::changeStep(const std::shared_ptr<Step> &s) {
    step_ = s;
}

//void Context::setTask(const Task &t){
//    task_ = t;
//}
//
//Task Context::getTask() const{
//    return task_;
//}

void Context::setTitle(const std::string &title){
    data_.title_ = title;
}
void Context::setDueDate(const time_t & dd){
    data_.due_date_ = dd;
}
void Context::setPriority(const Task::Priority &p){
    data_.priority_ = p;
}

void Context::setData(const Task::Data &d) {
    data_ = d;
}
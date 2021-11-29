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

void Context::setTitle(const std::string &title){
    data_.title = title;
}
void Context::setDueDate(const time_t & dd){
    data_.due_date = dd;
}
void Context::setPriority(const Task::Priority &p){
    data_.priority = p;
}

void Context::setData(const Task::Data &d) {
    data_ = d;
}

void Context::setID(const std::optional<TaskID> &id) {
    id_ = id;
}
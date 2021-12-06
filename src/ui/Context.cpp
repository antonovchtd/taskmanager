//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

std::shared_ptr<Step> Context::getStep() const {
    return step_;
}

std::shared_ptr<Step> Context::getOldStep() const {
    return oldStep_;
}

Task::Data Context::data() const {
    return data_;
}

std::optional<TaskID> Context::id() const {
    return id_;
}

std::map<TaskID, std::pair<Task, Node>> Context::getTasks() const {
    return tasks_;
}

std::string Context::label() const {
    return label_;
}

void Context::setStep(const std::shared_ptr<Step> &s) {
    setOldStep(getStep());
    step_ = s;
}

void Context::setOldStep(const std::shared_ptr<Step> &s) {
    oldStep_ = s;
}

void Context::revertStep() {
    step_ = oldStep_;
}

void Context::resetTaskData() {
    data_ = Task::Data{};
    id_ = TaskID::invalidID();
}

void Context::setTitle(const std::string &title) {
    data_.title = title;
}

void Context::setDueDate(const time_t & dd) {
    data_.due_date = dd;
}

void Context::setPriority(const Task::Priority &p) {
    data_.priority = p;
}

void Context::setData(const Task::Data &d) {
    data_ = d;
}

void Context::setID(const std::optional<TaskID> &id) {
    id_ = id;
}

void Context::setTasks(const std::map<TaskID, std::pair<Task, Node>> &tasks) {
    tasks_ = tasks;
}

void Context::setLabel(const std::string &s) {
    label_ = s;
}

void Context::setArg(const std::string &arg) {
    arg_ = arg;
}

const std::string &Context::arg() const {
    return arg_;
}

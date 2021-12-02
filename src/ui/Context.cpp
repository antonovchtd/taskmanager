//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

std::shared_ptr<Step> Context::getStep() {
    return step_;
}

std::shared_ptr<Step> Context::getOldStep() {
    return oldStep_;
}

Task::Data Context::data() {
    return data_;
}

std::optional<TaskID> Context::id() {
    return id_;
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

std::map<TaskID, std::pair<Task, Node>> Context::getTasks() {
    return tasks_;
}

void Context::setTasks(const std::map<TaskID, std::pair<Task, Node>> &tasks) {
    tasks_ = tasks;
}

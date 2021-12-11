//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

std::shared_ptr<Step> Context::step() const {
    return step_;
}

Task::Data Context::data() const {
    return data_;
}

std::optional<TaskID> Context::id() const {
    return id_;
}

std::map<TaskID, std::pair<Task, Node>> Context::tasks() const {
    return tasks_;
}

void Context::setStep(const std::shared_ptr<Step> &s) {
    step_ = s;
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
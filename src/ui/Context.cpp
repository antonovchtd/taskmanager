//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

std::shared_ptr<Step> Context::step() const {
    return step_;
}

ProtoTask::Task Context::task() const {
    return task_;
}

std::optional<ProtoTask::TaskID> Context::id() const {
    return id_;
}

std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> Context::tasks() const {
    return tasks_;
}

void Context::setStep(const std::shared_ptr<Step> &s) {
    step_ = s;
}

void Context::setTitle(const std::string &title) {
    task_.set_title(title);
}

void Context::setDueDate(const time_t & dd) {
//    auto ts = google::protobuf::util::TimeUtil::TimeTToTimestamp(dd);
    task_.set_due_date(dd);
}

void Context::setPriority(const ProtoTask::Task::Priority &p) {
    task_.set_priority(p);
}

void Context::setTask(const ProtoTask::Task &t) {
    task_ = t;
}

void Context::setID(const std::optional<ProtoTask::TaskID> &id) {
    id_ = id;
}

void Context::setTasks(const std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> &tasks) {
    tasks_ = tasks;
}
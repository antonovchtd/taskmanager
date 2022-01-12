//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

ProtoTask::Task Context::task() const {
    return task_;
}

std::optional<ProtoTask::TaskID> Context::id() const {
    return id_;
}

std::vector<ProtoTask::TaskEntity> Context::tasks() const {
    return tasks_;
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

void Context::setTasks(const std::vector<ProtoTask::TaskEntity> &tasks) {
    tasks_ = tasks;
}
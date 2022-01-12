//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

Core::Task Context::task() const {
    return task_;
}

std::optional<Core::TaskID> Context::id() const {
    return id_;
}

std::vector<Core::TaskEntity> Context::tasks() const {
    return tasks_;
}

void Context::setTitle(const std::string &title) {
    task_.set_title(title);
}

void Context::setDueDate(const time_t & dd) {
//    auto ts = google::protobuf::util::TimeUtil::TimeTToTimestamp(dd);
    task_.set_due_date(dd);
}

void Context::setPriority(const Core::Task::Priority &p) {
    task_.set_priority(p);
}

void Context::setTask(const Core::Task &t) {
    task_ = t;
}

void Context::setID(const std::optional<Core::TaskID> &id) {
    id_ = id;
}

void Context::setTasks(const std::vector<Core::TaskEntity> &tasks) {
    tasks_ = tasks;
}
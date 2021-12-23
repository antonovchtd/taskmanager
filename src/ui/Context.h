//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_UI_CONTEXT_H_
#define TASKMANAGER_SRC_UI_CONTEXT_H_

#include <memory>
#include <iostream>
#include <google/protobuf/util/time_util.h>

#include "Step.h"
#include "Factory.h"
#include "model/TaskManager.h"

class Context{
public:
    ProtoTask::Task task() const;
    std::optional<ProtoTask::TaskID> id() const;
    Container tasks() const;

public:
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const ProtoTask::Task::Priority &);
    void setTask(const ProtoTask::Task &);
    void setID(const std::optional<ProtoTask::TaskID> &);
    void setTasks(const Container &);

private:
    ProtoTask::Task task_;
    std::optional<ProtoTask::TaskID> id_;
    Container tasks_;
};

#endif //TASKMANAGER_SRC_UI_CONTEXT_H_

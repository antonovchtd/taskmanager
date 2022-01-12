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

class Context{
public:
    Core::Task task() const;
    std::optional<Core::TaskID> id() const;
    std::vector<Core::TaskEntity> tasks() const;

public:
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const Core::Task::Priority &);
    void setTask(const Core::Task &);
    void setID(const std::optional<Core::TaskID> &);
    void setTasks(const std::vector<Core::TaskEntity> &);

private:
    Core::Task task_;
    std::optional<Core::TaskID> id_;
    std::vector<Core::TaskEntity> tasks_;
};

#endif //TASKMANAGER_SRC_UI_CONTEXT_H_

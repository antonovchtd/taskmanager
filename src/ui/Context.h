//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_UI_CONTEXT_H_
#define TASKMANAGER_SRC_UI_CONTEXT_H_

#include <memory>
#include <iostream>

#include "Step.h"
#include "Factory.h"
#include "../model/TaskManager.h"

class Context{
public:
    std::shared_ptr<Step> step() const;
    Task::Data data() const;
    std::optional<TaskID> id() const;
    std::map<TaskID, std::pair<Task, Node>> tasks() const;

public:
    void setStep(const std::shared_ptr<Step> &);
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const Task::Priority &);
    void setData(const Task::Data &);
    void setID(const std::optional<TaskID> &);
    void setTasks(const std::map<TaskID, std::pair<Task, Node>> &);

private:
    Task::Data data_;
    std::optional<TaskID> id_;
    std::shared_ptr<Step> step_;
    std::map<TaskID, std::pair<Task, Node>> tasks_;
};

#endif //TASKMANAGER_SRC_UI_CONTEXT_H_

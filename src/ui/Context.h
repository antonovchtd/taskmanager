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
    std::shared_ptr<Step> getStep();
    std::shared_ptr<Step> getOldStep();
    Task::Data data();
    std::optional<TaskID> id();
    std::map<TaskID, std::pair<Task, Node>> getTasks();

public:
    void setStep(const std::shared_ptr<Step> &);
    void setOldStep(const std::shared_ptr<Step> &);
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const Task::Priority &);
    void setData(const Task::Data &);
    void setID(const std::optional<TaskID> &);
    void setTasks(const std::map<TaskID, std::pair<Task, Node>> &);

public:
    void revertStep();
    void resetTaskData();

private:
    Task::Data data_;
    std::optional<TaskID> id_;
    std::map<TaskID, std::pair<Task, Node>> tasks_;
    std::shared_ptr<Step> step_;
    std::shared_ptr<Step> oldStep_;
};

#endif //TASKMANAGER_SRC_UI_CONTEXT_H_

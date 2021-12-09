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
    std::shared_ptr<Step> getStep() const;
    Task::Data data() const;
    std::optional<TaskID> id() const;
    std::map<TaskID, std::pair<Task, Node>> getTasks() const;
    std::string label() const;
    const std::string &arg() const;
    bool askConfirmation() const;

public:
    void setStep(const std::shared_ptr<Step> &);
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const Task::Priority &);
    void setData(const Task::Data &);
    void setID(const std::optional<TaskID> &);
    void setTasks(const std::map<TaskID, std::pair<Task, Node>> &);
    void setLabel(const std::string &);
    void setArg(const std::string &arg);
    void setAskConfirmation(bool);

public:
    void resetTaskData();

private:
    Task::Data data_;
    std::optional<TaskID> id_;
    std::map<TaskID, std::pair<Task, Node>> tasks_;
    std::string label_;
    std::string arg_;
    bool askConfirmation_;
    std::shared_ptr<Step> step_;
};

#endif //TASKMANAGER_SRC_UI_CONTEXT_H_

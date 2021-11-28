//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_UI_CONTEXT_H_
#define TASKMANAGER_SRC_UI_CONTEXT_H_

#include <memory>
#include <iostream>

#include "Step.h"
#include "StepFactory.h"
#include "../model/TaskManager.h"

class Context{
public:
    virtual void execute(StepFactory &f);
    void changeStep(const std::shared_ptr<Step> &);

    std::shared_ptr<Step> getStep () { return step_; };
    Task::Data data() { return data_; };
    std::optional<TaskID> id() { return id_; };
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const Task::Priority &);
    void setData(const Task::Data &);
    void setID(const std::optional<TaskID> &);
    virtual ~Context() = default;

private:
    //TODO not using friends ?
    friend class AddTaskStep;
    friend class EditTaskStep;
    friend class AddSubtaskStep;
    friend class ShowStep;
    friend class ReadIDStep;
    friend class CompleteStep;
    friend class DeleteStep;

    Task::Data data_;
    std::optional<TaskID> id_ = TaskID::invalidID();
    std::shared_ptr<Step> step_;
    //TODO TaskManager has to be removed from context ?
    std::shared_ptr<TaskManager> man_ = std::shared_ptr<TaskManager>{new TaskManager};
};

#endif //TASKMANAGER_SRC_UI_CONTEXT_H_

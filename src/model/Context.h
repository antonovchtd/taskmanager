//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_MODEL_CONTEXT_H_
#define TASKMANAGER_SRC_MODEL_CONTEXT_H_

#include <memory>
#include <iostream>

#include "State.h"
#include "StateFactory.h"
#include "TaskManager.h"

class Context{
public:
    virtual void execute(StateFactory &f);
    void changeState(const std::shared_ptr<State> &);

    std::shared_ptr<State> getState () { return state_; };
    void setTitle(const std::string &);
    void setDueDate(const time_t &);
    void setPriority(const Task::Priority &);
    virtual ~Context() = default;

private:
    //TODO for testing/development
    friend class AddTaskState;
    friend class ShowState;

    Task::Data data_;
    std::shared_ptr<State> state_;
    //TODO TaskManager has to be removed from context
    std::shared_ptr<TaskManager> man_ = std::shared_ptr<TaskManager>{new TaskManager};
};

#endif //TASKMANAGER_SRC_MODEL_CONTEXT_H_

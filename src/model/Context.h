//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_MODEL_CONTEXT_H_
#define TASKMANAGER_SRC_MODEL_CONTEXT_H_

#include <memory>
#include <iostream>

#include "State.h"
#include "TaskManager.h"

class Context{
public:
    virtual void execute();
    void changeState(const std::shared_ptr<State> &);
    std::shared_ptr<State> getState () { return state_; };
//    void setTask(const Task &);
//    Task getTask() const;
    void setTitle(const std::string &);
    void setDueDate(const std::string &);
    void setPriority(const Task::Priority &);

private:
    friend class AddTaskState;
    friend class ShowState;
    std::shared_ptr<State> state_;
//    Task task_ = Task{};
    std::string title_;
    std::string due_date_;
    Task::Priority priority_;
    std::shared_ptr<TaskManager> man_ = std::shared_ptr<TaskManager>{new TaskManager};
};

#endif //TASKMANAGER_SRC_MODEL_CONTEXT_H_

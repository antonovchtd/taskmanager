//
// Created by Anton O. on 11/30/21.
//

#ifndef TASKMANAGER_SRC_ACTION_H_
#define TASKMANAGER_SRC_ACTION_H_

#include "../model/TaskManager.h"
#include "../ui/Context.h"

class Action {
public:
    Action() = default;
    virtual void make(TaskManager &, Context &) = 0;
};

class AddTaskAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class ValidateIDAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class ValidateNoIDAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class EditTaskAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class AddSubtaskAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class ShowAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class CompleteTaskAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};

class DeleteTaskAction : public Action {
public:
    void make(TaskManager &, Context &) override;
};


#endif //TASKMANAGER_SRC_ACTION_H_

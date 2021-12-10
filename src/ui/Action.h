//
// Created by Anton O. on 11/30/21.
//

#ifndef TASKMANAGER_SRC_UI_ACTION_H_
#define TASKMANAGER_SRC_UI_ACTION_H_

#include "../model/TaskManager.h"
//#include "Context.h"

class Context;

class Action {
public:
    struct Data {
        std::string arg;
    };

public:
    explicit Action(const std::shared_ptr<TaskManager> &);
    Action(const std::shared_ptr<TaskManager> &, const Data &);
    virtual void make(Context &) = 0;
    
public:
    std::shared_ptr<TaskManager> model() const;
    Action::Data data() const;
    void setActionData(const Action::Data &data);

private:
    std::shared_ptr<TaskManager> model_;
    Action::Data data_;
};

class DoNothingAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class AddTaskAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class ValidateIDAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class ValidateNoArgAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class ValidateLabelArgAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class EditTaskAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class AddSubtaskAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class ShowAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class CompleteTaskAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class DeleteAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class ConfirmDeleteAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};

class LabelAction : public Action {
public:
    using Action::Action;
    void make(Context &) override;
};


#endif //TASKMANAGER_SRC_UI_ACTION_H_

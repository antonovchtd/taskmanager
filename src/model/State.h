//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_MODEL_STATE_H_
#define TASKMANAGER_SRC_MODEL_STATE_H_

#include <string>
#include <memory>
#include <iostream>
#include <fstream>

#include "StateFactory.h"

class Context;

class State{
public:
    virtual void execute(Context &c) = 0;
    static std::string readline(const std::string &prompt);

protected:
    void changeState(const std::shared_ptr<Context> &, std::shared_ptr<State>);
};

class HomeState : public State{
    virtual void execute(Context &c) override;
};

class HelpState : public State{
    virtual void execute(Context &c) override;
};

class AddState : public State{
    virtual void execute(Context &c) override;

};

class ReadTitleState : public State{
    virtual void execute(Context &c) override;

};

class ReadPriorityState : public State{
    virtual void execute(Context &c) override;

};

class ReadDueDateState : public State{
    virtual void execute(Context &c) override;

};

class QuitState : public State{
    virtual void execute(Context &c) override;
};

class AddTaskState : public State{
    virtual void execute(Context &c) override;
};

class ShowState : public State{
    virtual void execute(Context &c) override;
};

#endif //TASKMANAGER_SRC_MODEL_STATE_H_

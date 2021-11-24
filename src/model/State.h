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
    virtual void execute(Context &c, StateFactory &f) = 0;
    static std::string readline(const std::string &prompt);

protected:
    void changeState(const std::shared_ptr<Context> &, std::shared_ptr<State>);
};

class HomeState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class HelpState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class AddState : public State{
    void execute(Context &c, StateFactory &f) override;

};

class ReadTitleState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class ReadPriorityState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class ReadDueDateState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class QuitState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class AddTaskState : public State{
    void execute(Context &c, StateFactory &f) override;
};

class ShowState : public State{
    void execute(Context &c, StateFactory &f) override;
};

#endif //TASKMANAGER_SRC_MODEL_STATE_H_

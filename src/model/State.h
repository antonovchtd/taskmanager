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
    virtual std::string classname() const { return "State"; }

protected:
    void changeState(const std::shared_ptr<Context> &, std::shared_ptr<State>);
};

class HomeState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "HomeState"; }
};

class HelpState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "HelpState"; }
};

class AddState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "AddState"; }

};

class ReadTitleState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "ReadTitleState"; }

};

class ReadPriorityState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "ReadPriorityState"; }

};

class ReadDueDateState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "ReadDueDateState"; }

};

class QuitState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "QuitState"; }
};

class AddTaskState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "AddTaskState"; }
};

class ShowState : public State{
    void execute(Context &c, StateFactory &f) override;
    std::string classname() const override { return "ShowState"; }
};

#endif //TASKMANAGER_SRC_MODEL_STATE_H_

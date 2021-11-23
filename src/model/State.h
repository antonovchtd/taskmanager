//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_MODEL_STATE_H_
#define TASKMANAGER_SRC_MODEL_STATE_H_

#include <string>
#include <memory>
#include <iostream>
#include <fstream>

class Context;

class State{
public:
    virtual void execute(Context* c) = 0;
    std::string readline(const std::string &prompt);

protected:
    void changeState(std::shared_ptr<Context>, std::shared_ptr<State>);
};

class HomeState : public State{
    virtual void execute(Context* c) override;
};

class HelpState : public State{
    virtual void execute(Context* c) override;
};

class AddState : public State{
    virtual void execute(Context* c) override;
};

class QuitState : public State{
    virtual void execute(Context* c) override;
};

#endif //TASKMANAGER_SRC_MODEL_STATE_H_

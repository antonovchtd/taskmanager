//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_MODEL_CONTEXT_H_
#define TASKMANAGER_SRC_MODEL_CONTEXT_H_

#include <memory>
#include <iostream>

#include "State.h"

class Context{
public:
    virtual void execute();
    void changeState(const std::shared_ptr<State> &);
    std::shared_ptr<State> getState () { return state_; };

private:
    std::shared_ptr<State> state_;
};

#endif //TASKMANAGER_SRC_MODEL_CONTEXT_H_

//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_MODEL_STATEFACTORY_H_
#define TASKMANAGER_SRC_MODEL_STATEFACTORY_H_

#include <memory>
#include <string>
#include "State.h"

class State;

class StateFactory{
public:
    static std::shared_ptr<State> create(const std::string &);
};

#endif //TASKMANAGER_SRC_MODEL_STATEFACTORY_H_

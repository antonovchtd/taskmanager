//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_ACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_ACTION_H_

#include "ActionResult.h"
#include "model/ModelInterface.h"

class Context;

class Action {
public:
    virtual ActionResult execute(const std::shared_ptr<ModelInterface> &) = 0;
    virtual ~Action() = default;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_ACTION_H_

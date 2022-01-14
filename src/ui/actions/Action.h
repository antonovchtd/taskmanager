//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_ACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_ACTION_H_

#include "utilities/ActionResult.h"
#include "model/ModelInterface.h"
#include "ui/Context.h"

class Action {
public:
    virtual ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) = 0;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_ACTION_H_

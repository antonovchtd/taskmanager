//
// Created by Anton O. on 11/30/21.
//

#ifndef TASKMANAGER_SRC_CONTROLLER_H_
#define TASKMANAGER_SRC_CONTROLLER_H_

#include "../model/TaskManager.h"
#include "../ui/Context.h"
#include "Action.h"

class Controller {
public:
    void Accept(TaskManager &, Context &c, const std::shared_ptr<Action> &);
};


#endif //TASKMANAGER_SRC_CONTROLLER_H_

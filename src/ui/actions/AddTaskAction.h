//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_ADDTASKACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_ADDTASKACTION_H_

#include "Action.h"

class AddTaskAction : public Action {
public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;
};

#endif //TASKMANAGER_SRC_UI_ACTIONS_ADDTASKACTION_H_

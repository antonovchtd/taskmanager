//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_ADDTASKACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_ADDTASKACTION_H_

#include "Action.h"

class AddTaskAction : public Action {
public:
    AddTaskAction(const Core::Task &);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::Task task_;
};

#endif //TASKMANAGER_SRC_UI_ACTIONS_ADDTASKACTION_H_

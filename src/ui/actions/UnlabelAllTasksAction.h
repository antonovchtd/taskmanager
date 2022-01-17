//
// Created by Anton O. on 1/17/22.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_UNLABELALLTASKSACTION_H
#define TASKMANAGER_SRC_UI_ACTIONS_UNLABELALLTASKSACTION_H

#include "Action.h"

class UnlabelAllTasksAction : public Action {
public:
    UnlabelAllTasksAction(const Core::TaskID &);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_UNLABELALLTASKSACTION_H

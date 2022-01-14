//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_RC_UI_ACTIONS_DELETETASKACTION_H_
#define TASKMANAGER_RC_UI_ACTIONS_DELETETASKACTION_H_

#include "Action.h"

class DeleteTaskAction : public Action {
public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;
};


#endif //TASKMANAGER_RC_UI_ACTIONS_DELETETASKACTION_H_

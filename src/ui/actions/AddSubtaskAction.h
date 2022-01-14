//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_
#define TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_

#include "Action.h"

class AddSubtaskAction : public Action {
public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;
};


#endif //TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_

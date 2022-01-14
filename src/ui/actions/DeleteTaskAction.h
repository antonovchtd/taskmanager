//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_RC_UI_ACTIONS_DELETETASKACTION_H_
#define TASKMANAGER_RC_UI_ACTIONS_DELETETASKACTION_H_

#include "Action.h"

class DeleteTaskAction : public Action {
public:
    DeleteTaskAction(const Core::TaskID &);
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
};


#endif //TASKMANAGER_RC_UI_ACTIONS_DELETETASKACTION_H_

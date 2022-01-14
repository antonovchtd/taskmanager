//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_
#define TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_

#include "Action.h"

class AddSubtaskAction : public Action {
public:
    AddSubtaskAction(const Core::Task &, const Core::TaskID &);
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::Task task_;
    Core::TaskID id_;
};


#endif //TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_

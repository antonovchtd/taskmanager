//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_
#define TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_

#include "Action.h"

class AddSubtaskAction : public Action {
public:
    AddSubtaskAction(const Core::TaskID &, const Core::Task &);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
    Core::Task task_;
};


#endif //TASKMANAGER_RC_UI_ACTIONS_ADDSUBTASKACTION_H_

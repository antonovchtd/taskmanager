//
// Created by Anton O. on 1/17/22.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_CLEARLABELOFTASKACTTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_CLEARLABELOFTASKACTTION_H_

#include "Action.h"

class ClearLabelOfTaskAction : public Action {
public:
    explicit ClearLabelOfTaskAction(const Core::TaskID &, const Core::Label &label);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
    Core::Label label_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_CLEARLABELOFTASKACTTION_H_
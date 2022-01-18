//
// Created by Anton O. on 1/17/22.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_UNLABELTASKACTTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_UNLABELTASKACTTION_H_

#include "Action.h"

class ClearLabelOfTaskAction : public Action {
public:
    ClearLabelOfTaskAction(const Core::TaskID &, const std::string &label);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
    std::string label_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_UNLABELTASKACTTION_H_

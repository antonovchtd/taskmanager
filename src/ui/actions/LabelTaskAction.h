//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_LABELTASKACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_LABELTASKACTION_H_

#include "Action.h"

class LabelTaskAction : public Action {
public:
    LabelTaskAction(const Core::TaskID &, const std::string &label);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
    std::string label_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_LABELTASKACTION_H_

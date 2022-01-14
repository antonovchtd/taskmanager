//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_VALIDATENOARGACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_VALIDATENOARGACTION_H_

#include "Action.h"

class ValidateNoArgAction : public Action {
public:
    ValidateNoArgAction(const std::string &arg);

public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;

private:
    std::string arg_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_VALIDATENOARGACTION_H_

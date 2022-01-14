//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_VALIDATEIDACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_VALIDATEIDACTION_H_

#include "Action.h"

class ValidateIDAction : public Action {
public:
    ValidateIDAction(const std::string &arg);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::string arg_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_VALIDATEIDACTION_H_

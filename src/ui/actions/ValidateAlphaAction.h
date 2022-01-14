//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_VALIDATEALPHAACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_VALIDATEALPHAACTION_H_

#include "Action.h"

class ValidateAlphaAction : public Action {
public:
    ValidateAlphaAction(const std::string &arg);

public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;

private:
    std::string arg_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_VALIDATEALPHAACTION_H_

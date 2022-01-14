//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_VALIDATEALPHAORIDACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_VALIDATEALPHAORIDACTION_H_

#include "Action.h"

class ValidateAlphaOrIDAction : public Action {
public:
    ValidateAlphaOrIDAction(const std::string &arg);

public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;

private:
    std::string arg_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_VALIDATEALPHAORIDACTION_H_

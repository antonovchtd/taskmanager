//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_VALIDATEIDACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_VALIDATEIDACTION_H_

#include "Action.h"

class ValidateIDAction : public Action {
public:
    explicit ValidateIDAction(const std::optional<Core::TaskID> &id);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::optional<Core::TaskID> id_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_VALIDATEIDACTION_H_

//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_COMPLETETASKACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_COMPLETETASKACTION_H_

#include "Action.h"

class CompleteTaskAction : public Action {
public:
    explicit CompleteTaskAction(const Core::TaskID &);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_COMPLETETASKACTION_H_

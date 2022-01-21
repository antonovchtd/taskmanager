//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_STEP_H_
#define TASKMANAGER_SRC_UI_STEPS_STEP_H_

#include "ui/actions/Action.h"

class Factory;

class Step {
public:
    virtual std::unique_ptr<Action> genAction(Context &) = 0;
    virtual std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) = 0;
    virtual ~Step() = default;

};


#endif //TASKMANAGER_SRC_UI_STEPS_STEP_H_

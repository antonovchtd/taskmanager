//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_UNCOMPLETESTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_UNCOMPLETESTEP_H_

#include "IOStep.h"
#include "utilities/StepUtils.h"
#include "ui/actions/UncompleteTaskAction.h"

class UncompleteStep : public IOStep {
public:
    using IOStep::IOStep;
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;
};

#endif //TASKMANAGER_SRC_UI_STEPS_UNCOMPLETESTEP_H_

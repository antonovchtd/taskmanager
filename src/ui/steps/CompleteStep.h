//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_COMPLETESTEP_H_
#define TASKMANAGER_SRC_UI_ACTIONS_COMPLETESTEP_H_

#include "Step.h"
#include "utilities/StepUtils.h"
#include "ui/actions/CompleteTaskAction.h"

class CompleteStep : public Step {
public:
    using Step::Step;
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_COMPLETESTEP_H_

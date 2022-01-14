//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_SHOWSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_SHOWSTEP_H_

#include "Step.h"
#include "utilities/StepUtils.h"
#include "utilities/TaskUtils.h"
#include "utilities/TaskIDUtils.h"
#include "ui/actions/DoNothingAction.h"

class ShowStep : public Step {
public:
    using Step::Step;
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

public:
    void recursivePrint(const Core::TaskEntity &,
                        const Context &,
                        const std::string &prefix);
};


#endif //TASKMANAGER_SRC_UI_STEPS_SHOWSTEP_H_

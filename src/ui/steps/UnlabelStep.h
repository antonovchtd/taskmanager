//
// Created by Anton O. on 1/17/22.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_UNLABELSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_UNLABELSTEP_H_

#include "Step.h"
#include "utilities/StepUtils.h"
#include "ui/actions/UnlabelTaskAction.h"

class UnlabelStep : public Step {
public:
    using Step::Step;
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

public:
    std::string readLabel() const;
};


#endif //TASKMANAGER_SRC_UI_STEPS_UNLABELSTEP_H_

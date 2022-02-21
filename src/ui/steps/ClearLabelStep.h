
//
// Created by Anton O. on 1/17/22.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_CLEARLABELSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_CLEARLABELSTEP_H_

#include "IOStep.h"
#include "utilities/StepUtils.h"
#include "ui/actions/ClearLabelOfTaskAction.h"

class ClearLabelStep : public IOStep {
public:
    ClearLabelStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer);
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

public:
    Core::Label readLabel() const;
};


#endif //TASKMANAGER_SRC_UI_STEPS_CLEARLABELSTEP_H_
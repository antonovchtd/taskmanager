//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_CONFIRMDELETESTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_CONFIRMDELETESTEP_H_

#include "IOStep.h"
#include "utilities/StepUtils.h"
#include "ui/actions/DoNothingAction.h"

class ConfirmDeleteStep : public IOStep {
public:
    ConfirmDeleteStep(const std::shared_ptr<AbstractReader> &reader,
                 const std::shared_ptr<AbstractPrinter> &printer);
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

public:
    std::shared_ptr<Step> getConfirmation(const std::shared_ptr<Factory> &f);
};


#endif //TASKMANAGER_SRC_UI_STEPS_CONFIRMDELETESTEP_H_

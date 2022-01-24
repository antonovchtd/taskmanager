//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_HELPSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_HELPSTEP_H_

#include <sstream>

#include "IOStep.h"
#include "utilities/StepUtils.h"
#include "ui/actions/DoNothingAction.h"

class HelpStep : public IOStep {
public:
    HelpStep(const std::shared_ptr<AbstractReader> &reader,
             const std::shared_ptr<AbstractPrinter> &printer);
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

};


#endif //TASKMANAGER_SRC_UI_STEPS_HELPSTEP_H_

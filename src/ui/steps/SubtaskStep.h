//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_SUBTASKSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_SUBTASKSTEP_H_

#include "Step.h"
#include "utilities/StepUtils.h"
#include "ui/Machine.h"
#include "ui/actions/AddSubtaskAction.h"

class SubtaskStep : public Step {
public:
    SubtaskStep(std::shared_ptr<AbstractReader> reader,
                std::shared_ptr<AbstractPrinter> printer,
                std::shared_ptr<Machine> submachine);

public:
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

private:
    std::shared_ptr<Machine> submachine_;
};


#endif //TASKMANAGER_SRC_UI_STEPS_SUBTASKSTEP_H_

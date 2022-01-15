//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_EDITSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_EDITSTEP_H_

#include "Step.h"
#include "utilities/StepUtils.h"
#include "ui/Machine.h"
#include "ui/actions/EditTaskAction.h"

class EditStep : public Step {
public:
    EditStep(std::shared_ptr<AbstractReader> reader,
            std::shared_ptr<AbstractPrinter> printer,
            Machine submachine);

public:
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

private:
    Machine submachine_;
};


#endif //TASKMANAGER_SRC_UI_STEPS_EDITSTEP_H_
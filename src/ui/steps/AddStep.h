//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_ADDSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_ADDSTEP_H_

#include "PrinterStep.h"
#include "utilities/StepUtils.h"
#include "ui/Machine.h"
#include "ui/actions/AddTaskAction.h"

class AddStep : public PrinterStep {
public:
    AddStep(const std::shared_ptr<AbstractPrinter> &printer,
            const std::shared_ptr<Machine> &submachine);

public:
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

private:
    std::shared_ptr<Machine> submachine_;
};


#endif //TASKMANAGER_SRC_UI_STEPS_ADDSTEP_H_

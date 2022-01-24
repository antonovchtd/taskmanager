//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "EditStep.h"

EditStep::EditStep(const std::shared_ptr<AbstractPrinter> &printer,
                   const std::shared_ptr<Machine> &submachine) :
        PrinterStep(printer) {
    submachine_ = submachine;
}


std::unique_ptr<Action> EditStep::genAction(Context &context) {
    printer()->print("[Edit Task]\n");

    Context input_context = submachine_->run(context);

    return std::unique_ptr<Action>(new EditTaskAction(*context.id(), input_context.task()));
}

std::shared_ptr<Step> EditStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Edited Task");
}

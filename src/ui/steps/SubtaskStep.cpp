//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "SubtaskStep.h"

SubtaskStep::SubtaskStep(const std::shared_ptr<AbstractReader> &reader,
                         const std::shared_ptr<AbstractPrinter> &printer,
                         const std::shared_ptr<Machine> &submachine) :
        IOStep(reader, printer) {
    submachine_ = submachine;
}


std::unique_ptr<Action> SubtaskStep::genAction(Context &context) {
    printer()->print("[Add Subtask]\n");

    Context input_context = submachine_->run();

    return std::unique_ptr<Action>(new AddSubtaskAction(*context.id(), input_context.task()));
}

std::shared_ptr<Step> SubtaskStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Added Subtask");
}

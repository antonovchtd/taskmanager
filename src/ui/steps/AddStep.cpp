//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddStep.h"

AddStep::AddStep(std::shared_ptr<AbstractReader> reader,
                 std::shared_ptr<AbstractPrinter> printer,
                 Machine submachine) :
         Step(reader, printer) {
    submachine_ = submachine;
}


std::unique_ptr<Action> AddStep::genAction(Context &context) {
    printer()->print("[Add Task]\n");
    Context input_context = submachine_.run();
    context.setTask(input_context.task());

    return std::unique_ptr<Action>(new AddTaskAction);
}

std::shared_ptr<Step> AddStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Added Task");
}

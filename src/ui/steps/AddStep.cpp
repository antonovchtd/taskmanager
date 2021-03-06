//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddStep.h"

AddStep::AddStep(const std::shared_ptr<AbstractPrinter> &printer,
                 const std::shared_ptr<Machine> &submachine) :
         PrinterStep(printer), submachine_{submachine} {

}


std::unique_ptr<Action> AddStep::genAction(Context &) {
    printer()->print("[Add Task]\n");
    Context input_context = submachine_->run();

    return std::unique_ptr<Action>(new AddTaskAction(input_context.task()));
}

std::shared_ptr<Step> AddStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Added Task");
}

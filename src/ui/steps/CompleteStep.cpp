//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "CompleteStep.h"
#include "ui/Context.h"

CompleteStep::CompleteStep(const std::shared_ptr<AbstractPrinter> &printer) :
        PrinterStep(printer) {
}

std::unique_ptr<Action> CompleteStep::genAction(Context &context) {
    return std::unique_ptr<Action>(new CompleteTaskAction(*context.id()));
}

std::shared_ptr<Step> CompleteStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Completed Task");
}


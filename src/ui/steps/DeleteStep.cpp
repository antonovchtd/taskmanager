//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DeleteStep.h"
#include "ui/Context.h"

DeleteStep::DeleteStep(const std::shared_ptr<AbstractPrinter> &printer) :
        PrinterStep(printer) {
}

std::unique_ptr<Action> DeleteStep::genAction(Context &context) {
    return std::unique_ptr<Action>(new DeleteTaskAction(*context.id()));
}

std::shared_ptr<Step> DeleteStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Deleted Task");
}


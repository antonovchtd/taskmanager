//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "QuitStep.h"

QuitStep::QuitStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer) :
        IOStep(reader, printer) {
}

std::unique_ptr<Action> QuitStep::genAction(Context &context) {
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> QuitStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");
}
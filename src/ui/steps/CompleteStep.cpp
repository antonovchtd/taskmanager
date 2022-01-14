//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "CompleteStep.h"

std::unique_ptr<Action> CompleteStep::genAction(Context &) {
    return std::unique_ptr<Action>(new CompleteTaskAction);
}

std::shared_ptr<Step> CompleteStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Completed Task");
}


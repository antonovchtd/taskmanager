//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "UncompleteStep.h"

std::unique_ptr<Action> UncompleteStep::genAction(Context &) {
    return std::unique_ptr<Action>(new UncompleteTaskAction);
}

std::shared_ptr<Step> UncompleteStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Uncompleted Task");
}


//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DeleteStep.h"

std::unique_ptr<Action> DeleteStep::genAction(Context &) {
    return std::unique_ptr<Action>(new DeleteTaskAction);
}

std::shared_ptr<Step> DeleteStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Deleted Task");
}


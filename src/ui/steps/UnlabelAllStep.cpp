//
// Created by Anton O. on 1/17/22.
//

#include "UnlabelAllStep.h"
#include "ui/Context.h"

std::unique_ptr<Action> UnlabelAllStep::genAction(Context &context) {
    return std::unique_ptr<Action>(new ClearAllLabelsOfTaskAction(*context.id()));
}

std::shared_ptr<Step> UnlabelAllStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Removed all labels of the task");

}
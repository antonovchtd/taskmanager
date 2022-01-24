//
// Created by Anton O. on 1/17/22.
//

#include "ClearAllLabelsStep.h"
#include "ui/Context.h"

std::unique_ptr<Action> ClearAllLabelsStep::genAction(Context &context) {
    return std::unique_ptr<Action>(new ClearAllLabelsOfTaskAction(*context.id()));
}

std::shared_ptr<Step> ClearAllLabelsStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Removed all labels of the task");

}

//
// Created by Anton O. on 1/17/22.
//

#include "ClearLabelStep.h"
#include "ui/Context.h"

std::unique_ptr<Action> ClearLabelStep::genAction(Context &context) {
    std::string label = readLabel();
    return std::unique_ptr<Action>(new ClearLabelOfTaskAction(*context.id(), label));
}

std::shared_ptr<Step> ClearLabelStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Removed label from task");

}

std::string ClearLabelStep::readLabel() const {
    std::string label = reader()->read("[Clear Label]\n    >> ");
    while (label.empty())
        label = reader()->read("Label cannot be empty. Try again.\n    >> ");
    return label;
}
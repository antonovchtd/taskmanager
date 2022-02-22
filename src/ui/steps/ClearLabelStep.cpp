//
// Created by Anton O. on 1/17/22.
//

#include "ClearLabelStep.h"
#include "ui/Context.h"

ClearLabelStep::ClearLabelStep(const std::shared_ptr<AbstractReader> &reader,
                               const std::shared_ptr<AbstractPrinter> &printer) :
                               IOStep(reader, printer) {
}

std::unique_ptr<Action> ClearLabelStep::genAction(Context &context) {
    auto label = readLabel();
    return std::make_unique<ClearLabelOfTaskAction>(*context.id(), label);
}

std::shared_ptr<Step> ClearLabelStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Removed label from task");

}

Core::Label ClearLabelStep::readLabel() const {
    Core::Label label;
    std::string str = reader()->read("[Clear Label]\n    >> ");
    while (str.empty())
        str = reader()->read("Label cannot be empty. Try again.\n    >> ");
    label.set_str(str);
    return label;
}
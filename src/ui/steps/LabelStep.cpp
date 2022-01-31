//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "LabelStep.h"
#include "ui/Context.h"

LabelStep::LabelStep(const std::shared_ptr<AbstractReader> &reader,
                     const std::shared_ptr<AbstractPrinter> &printer) :
        IOStep(reader, printer) {
}

std::unique_ptr<Action> LabelStep::genAction(Context &context) {
    std::string label = readLabel();
    return std::unique_ptr<Action>(new LabelTaskAction(*context.id(), label));
}

std::shared_ptr<Step> LabelStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Added label to Task");

}

std::string LabelStep::readLabel() const {
    std::string label = reader()->read("[Add Label]\n    >> ");
    while (label.empty())
        label = reader()->read("Label cannot be empty. Try again.\n    >> ");
    return label;
}
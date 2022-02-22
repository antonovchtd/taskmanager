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
    auto label = readLabel();
    return std::make_unique<LabelTaskAction>(*context.id(), label);
}

std::shared_ptr<Step> LabelStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "Added label to Task");

}

Core::Label LabelStep::readLabel() const {
    Core::Label label;
    std::string str = reader()->read("[Clear Label]\n    >> ");
    while (str.empty())
        str = reader()->read("Label cannot be empty. Try again.\n    >> ");
    label.set_str(str);
    return label;
}
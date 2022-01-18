//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "LabelsStep.h"
#include "ui/Context.h"

std::unique_ptr<Action> LabelsStep::genAction(Context &context) {
    auto labels = context.tasks()[0].data().labels();
    for (auto it = labels.begin(); it != labels.end(); ++it)
        printer()->print(*it + " ");
    if (!labels.empty())
        printer()->print("\n");
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> LabelsStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");

}
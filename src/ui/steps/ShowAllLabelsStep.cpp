//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "ShowAllLabelsStep.h"
#include "ui/Context.h"

std::unique_ptr<Action> ShowAllLabelsStep::genAction(Context &context) {
    if (!context.tasks().empty()) {
        auto labels = context.tasks()[0].data().labels();
        for (auto it = labels.begin(); it != labels.end(); ++it)
            printer()->print(*it + " ");
        if (!labels.empty())
            printer()->print("\n");
    }
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> ShowAllLabelsStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");

}

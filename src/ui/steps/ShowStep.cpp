//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ShowStep.h"

std::unique_ptr<Action> ShowStep::genAction(Context &) {
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> ShowStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    if (result.type_id == ActionResult::kEntity) {
        auto tasks = result.tasks;
        for (const auto &te: tasks) {
            if (!te.has_parent())
                recursivePrint(te, tasks, "");
        }
    }
    return processResult(*this, factory, result, "");
}

void ShowStep::recursivePrint(const Core::TaskEntity &te,
                              const std::vector<Core::TaskEntity> &vec,
                              const std::string &prefix) {
    printer()->print(prefix + std::to_string(te.id().value()) +
                        " – " + to_string(te.data()));

    printer()->print("\n");
    for (const auto &ch : vec) {
        if (ch.parent() == te.id())
            recursivePrint(ch, vec, prefix + "    ");
    }
}
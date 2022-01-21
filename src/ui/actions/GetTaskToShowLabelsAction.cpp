//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "GetTaskToShowLabelsAction.h"

GetTaskToShowLabelsAction::GetTaskToShowLabelsAction(const std::optional<Core::TaskID> &id) : id_{id} {
}

ActionResult GetTaskToShowLabelsAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (id_) {
        if (!model->Validate(*id_))
            return {ActionResult::Status::ID_NOT_FOUND, id_};
    } else {
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }

    std::vector<Core::TaskEntity> tasks = model->getTasks(*id_);
    tasks.erase(tasks.begin() + 1, tasks.end());
    return {ActionResult::Status::SUCCESS, tasks};
}

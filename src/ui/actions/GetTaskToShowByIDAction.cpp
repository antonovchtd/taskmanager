//
// Created by Anton Ovcharenko on 31.01.2022.
//

#include "GetTaskToShowByIDAction.h"

GetTaskToShowByIDAction::GetTaskToShowByIDAction(const Core::TaskID &id) : id_{id} {
}

ActionResult GetTaskToShowByIDAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (!model->IsPresent(id_))
        return {ActionResult::Status::ID_NOT_FOUND, id_};

    auto tasks = model->getTaskWithSubtasks(id_);

    return {ActionResult::Status::SUCCESS, tasks};
}

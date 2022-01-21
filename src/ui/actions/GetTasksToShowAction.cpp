//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "GetTasksToShowAction.h"

GetTasksToShowAction::GetTasksToShowAction(const std::optional<Core::TaskID> &id) : id_{id} {
}

GetTasksToShowAction::GetTasksToShowAction(const std::string &arg) : arg_{arg} {
}

ActionResult GetTasksToShowAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (id_ && !model->Validate(*id_))
        return {ActionResult::Status::ID_NOT_FOUND, id_};

    std::vector<Core::TaskEntity> tasks;
    if (!id_ && arg_.empty())
        tasks = model->getTasks();
    else if (id_)
        tasks = model->getTaskWithSubtasks(*id_);
    else
        tasks = model->getTasks(arg_);

    return {ActionResult::Status::SUCCESS, tasks};
}

//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "GetAllTasksToShowAction.h"

ActionResult GetAllTasksToShowAction::execute(const std::shared_ptr<ModelInterface> &model) {

    auto tasks = model->getTasks();

    return {ActionResult::Status::SUCCESS, tasks};
}

//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "GetAllTasksToShowAction.h"

ActionResult GetAllTasksToShowAction::execute(const std::shared_ptr<ModelInterface> &model) {
    ActionResult result{model->getTasks()};
    return result;
}

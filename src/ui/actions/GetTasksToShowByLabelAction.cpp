//
// Created by Anton Ovcharenko on 31.01.2022.
//

#include "GetTasksToShowByLabelAction.h"

GetTasksToShowByLabelAction::GetTasksToShowByLabelAction(const std::string &label) : label_{label} {
}

ActionResult GetTasksToShowByLabelAction::execute(const std::shared_ptr<ModelInterface> &model) {
    ActionResult result{model->getTasks(label_)};
    return result;
}

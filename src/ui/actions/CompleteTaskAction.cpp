//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "CompleteTaskAction.h"

CompleteTaskAction::CompleteTaskAction(const Core::TaskID &id) : id_{id} {
}

ActionResult CompleteTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->Complete(id_);
}
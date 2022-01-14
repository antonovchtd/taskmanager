//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DeleteTaskAction.h"

DeleteTaskAction::DeleteTaskAction(const Core::TaskID &id) : id_{id} {
}

ActionResult DeleteTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->Delete(id_, true);
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "UncompleteTaskAction.h"

UncompleteTaskAction::UncompleteTaskAction(const Core::TaskID &id) : id_{id} {
}

ActionResult UncompleteTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->Uncomplete(id_);
}
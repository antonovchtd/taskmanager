//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "UncompleteTaskAction.h"

ActionResult UncompleteTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->Uncomplete(*context.id());
}
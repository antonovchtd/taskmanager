//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "EditTaskAction.h"

ActionResult EditTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->Edit(*context.id(), context.task());
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DeleteTaskAction.h"
#include "ui/Context.h"

ActionResult DeleteTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->Delete(*context.id(), true);
}
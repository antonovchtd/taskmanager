//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "CompleteTaskAction.h"
#include "ui/Context.h"

ActionResult CompleteTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->Complete(*context.id());
}
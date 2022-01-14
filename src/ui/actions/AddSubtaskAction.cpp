//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddSubtaskAction.h"

ActionResult AddSubtaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->AddSubtask(context.task(), *context.id());
}
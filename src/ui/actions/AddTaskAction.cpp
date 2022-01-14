//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddTaskAction.h"
#include "ui/Context.h"

ActionResult AddTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->Add(context.task());
}
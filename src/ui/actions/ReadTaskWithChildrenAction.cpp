//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ReadTaskWithChildrenAction.h"
#include "ui/Context.h"

ActionResult ReadTaskWithChildrenAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    try {
        context.setTasks(model->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, *context.id()};
    }
}
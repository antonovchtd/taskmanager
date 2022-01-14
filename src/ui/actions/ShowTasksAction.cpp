//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ShowTasksAction.h"

ShowTasksAction::ShowTasksAction() = default;

ShowTasksAction::ShowTasksAction(const std::string &arg) : arg_{arg} {
}

ActionResult ShowTasksAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    if (arg_.empty())
        context.setTasks(model->getTasks());
    else if (context.id()) {
        context.setTasks(model->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    }
    else
        context.setTasks(model->getTasks(arg_));

    return {ActionResult::Status::SUCCESS, std::nullopt};
}
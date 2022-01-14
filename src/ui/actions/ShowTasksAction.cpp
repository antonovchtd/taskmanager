//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ShowTasksAction.h"
#include "ui/Context.h"

ShowTasksAction::ShowTasksAction() = default;

ShowTasksAction::ShowTasksAction(const std::string &arg) : arg_{arg} {
}

ActionResult ShowTasksAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    Core::TaskID id;
    try {
        id.set_value(std::stoi(arg_));
        if(model->Validate(id))
            context.setID(id);
    } catch (const std::invalid_argument &) {
        context.setID(std::nullopt);
    }

    std::vector<Core::TaskEntity> tasks;
    if (arg_.empty())
        tasks = model->getTasks();
    else if (context.id())
        tasks = model->getTasks(*context.id());
    else
        tasks = model->getTasks(arg_);

    context.setTasks(tasks);

    return {ActionResult::Status::SUCCESS, context.id()};
}
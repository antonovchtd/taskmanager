//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateIDAction.h"
#include "ui/Context.h"

ValidateIDAction::ValidateIDAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateIDAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    Core::TaskID id;
    if (arg_.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    try {
        id.set_value(std::stoi(arg_));
        context.setID(id);
        auto result = model->Validate(id);
        if (result)
            context.setTask(model->getTasks(id)[0].data());
        return result;
    } catch (const std::invalid_argument &) {
        context.setID(std::nullopt);
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateIDAction.h"
#include "ui/Context.h"

ValidateIDAction::ValidateIDAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateIDAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::TaskID id;
    if (arg_.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    try {
        id.set_value(std::stoi(arg_));
        auto result = model->Validate(id);
        if (result)
            return {result.status, model->getTasks(id)};
        else
            return {result.status, id};
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }
}
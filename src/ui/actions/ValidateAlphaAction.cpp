//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateAlphaAction.h"
#include "ui/Context.h"

ValidateAlphaAction::ValidateAlphaAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateAlphaAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    // empty is not OK
    Core::TaskID id;
    try {
        id.set_value(std::stoi(arg_));
        return {ActionResult::Status::TAKES_ALPHA_NOT_ID, id};
    } catch (const std::invalid_argument &) {
        if (arg_.empty())
            return {ActionResult::Status::TAKES_ALPHA, std::nullopt};
        else
            return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}
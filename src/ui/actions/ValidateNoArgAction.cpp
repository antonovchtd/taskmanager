//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateNoArgAction.h"

ValidateNoArgAction::ValidateNoArgAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateNoArgAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (arg_.empty())
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::TAKES_NO_ARG, std::nullopt};
}
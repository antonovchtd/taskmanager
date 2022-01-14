//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateAlphaOrIDAction.h"

ValidateAlphaOrIDAction::ValidateAlphaOrIDAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateAlphaOrIDAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    // empty is OK
    Core::TaskID id;
    try {
        id.set_value(std::stoi(arg_));
        context.setID(id);
        return model->Validate(id);
    } catch (const std::invalid_argument &) {
        context.setID(std::nullopt);
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}
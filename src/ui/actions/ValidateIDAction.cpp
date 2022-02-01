//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateIDAction.h"
#include "utilities/ModelInquiryResultUtils.h"

ValidateIDAction::ValidateIDAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateIDAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelInquiryResult result;
    Core::TaskID id;

    if (arg_.empty()) {
        result.set_status(Core::ModelInquiryResult_Status_TAKES_ARG);
        return result;
    }

    try {
        id.set_value(std::stoi(arg_));
        auto check = model->IsPresent(id);
        if (ToBool(check))
            return ActionResult(model->getTaskWithSubtasks(id));
        else
            return check;
    } catch (const std::invalid_argument &) {
        result.set_status(Core::ModelInquiryResult_Status_TAKES_ID);
        return result;
    }
}
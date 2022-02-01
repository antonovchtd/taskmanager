//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateNoArgAction.h"

ValidateNoArgAction::ValidateNoArgAction(const std::string &arg) : arg_{arg} {
}

ActionResult ValidateNoArgAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelInquiryResult result;

    if (arg_.empty())
        result.set_status(Core::ModelInquiryResult_Status_SUCCESS);
    else
        result.set_status(Core::ModelInquiryResult_Status_TAKES_NO_ARG);

    return result;
}
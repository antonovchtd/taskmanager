//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DoNothingAction.h"

ActionResult DoNothingAction::execute(const std::shared_ptr<ModelInterface> &) {
    Core::ModelInquiryResult result;
    result.set_status(Core::ModelInquiryResult_Status_SUCCESS);
    return result;
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DoNothingAction.h"

ActionResult DoNothingAction::execute(const std::shared_ptr<ModelInterface> &) {
    Core::ModelRequestResult result;
    result.set_status(Core::ModelRequestResult_Status_SUCCESS);
    return result;
}
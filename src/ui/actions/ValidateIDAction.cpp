//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateIDAction.h"
#include "utilities/ModelRequestResultUtils.h"

ValidateIDAction::ValidateIDAction(const std::optional<Core::TaskID> &id) : id_{id} {
}

ActionResult ValidateIDAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelRequestResult result;
    if (id_) {
        return model->IsPresent(*id_);
    } else {
        result.set_status(Core::ModelRequestResult_Status_TAKES_ID);
        return result;
    }
}
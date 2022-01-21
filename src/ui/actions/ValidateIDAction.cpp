//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ValidateIDAction.h"
#include "ui/Context.h"

ValidateIDAction::ValidateIDAction(const std::optional<Core::TaskID> &id) : id_{id} {
}

ActionResult ValidateIDAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (id_) {
        auto result = model->Validate(*id_);
        if (result)
            return {result.status, model->getTaskWithSubtasks(*id_)};
        else
            return {result.status, id_};
    } else {
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }
}
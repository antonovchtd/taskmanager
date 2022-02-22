//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "GetTaskToShowLabelsAction.h"
#include "utilities/ModelRequestResultUtils.h"

GetTaskToShowLabelsAction::GetTaskToShowLabelsAction(const std::optional<Core::TaskID> &id) : id_{id} {
}

ActionResult GetTaskToShowLabelsAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelRequestResult result;
    if (id_) {
        auto check = model->CheckTask(*id_);
        if (!ToBool(check))
            return check;
    } else {
        result.set_status(Core::ModelRequestResult_Status_TAKES_ID);
        return result;
    }

    std::vector<Core::TaskEntity> tasks = model->getTaskWithSubtasks(*id_);
    if (!tasks.empty())
        return ActionResult(tasks[0]);
    else {
        result.set_status(Core::ModelRequestResult_Status_ID_NOT_FOUND);
        return result;
    }
}

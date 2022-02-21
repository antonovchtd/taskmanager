//
// Created by Anton Ovcharenko on 31.01.2022.
//

#include "GetTaskToShowByIDAction.h"
#include "utilities/ModelRequestResultUtils.h"

GetTaskToShowByIDAction::GetTaskToShowByIDAction(const Core::TaskID &id) : id_{id} {
}

ActionResult GetTaskToShowByIDAction::execute(const std::shared_ptr<ModelInterface> &model) {
    auto check = model->IsPresent(id_);
    if (!ToBool(check))
        return check;

    ActionResult result{model->getTaskWithSubtasks(id_)};
    return result;
}

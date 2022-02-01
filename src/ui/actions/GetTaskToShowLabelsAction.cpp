//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "GetTaskToShowLabelsAction.h"
#include "utilities/ModelInquiryResultUtils.h"

GetTaskToShowLabelsAction::GetTaskToShowLabelsAction(const std::string &arg) : arg_{arg} {
}

ActionResult GetTaskToShowLabelsAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelInquiryResult result;
    Core::TaskID id;
    try {
        id.set_value(std::stoi(arg_));
        auto check = model->IsPresent(id);
        if (!ToBool(check))
            return check;
    } catch (const std::invalid_argument &) {
        result.set_status(Core::ModelInquiryResult_Status_TAKES_ID);
        return result;
    }

    std::vector<Core::TaskEntity> tasks = model->getTaskWithSubtasks(id);
    if (!tasks.empty())
        return ActionResult(tasks[0]);
    else {
        result.set_status(Core::ModelInquiryResult_Status_ID_NOT_FOUND);
        return result;
    }
}

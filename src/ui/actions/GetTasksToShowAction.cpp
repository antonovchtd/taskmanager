//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "GetTasksToShowAction.h"
#include "utilities/ModelInquiryResultUtils.h"

GetTasksToShowAction::GetTasksToShowAction(const std::string &arg) : arg_{arg} {
}

ActionResult GetTasksToShowAction::execute(const std::shared_ptr<ModelInterface> &model) {
    std::optional<Core::TaskID> id{Core::TaskID()};
    try {
        id->set_value(std::stoi(arg_));
        auto check = model->IsPresent(*id);
        if (!ToBool(check))
            return check;
    } catch (const std::invalid_argument &) {
        id = std::nullopt;
    }

    std::vector<Core::TaskEntity> tasks;
    if (arg_.empty())
        tasks = model->getTasks();
    else if (id)
        tasks = model->getTaskWithSubtasks(*id);
    else
        tasks = model->getTasks(arg_);

    return ActionResult(tasks);
}

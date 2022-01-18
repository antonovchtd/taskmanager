//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "GetTaskToShowLabelsAction.h"

GetTaskToShowLabelsAction::GetTaskToShowLabelsAction(const std::string &arg) : arg_{arg} {
}

ActionResult GetTaskToShowLabelsAction::execute(const std::shared_ptr<ModelInterface> &model) {
    std::optional<Core::TaskID> id{Core::TaskID()};
    try {
        id->set_value(std::stoi(arg_));
        if (!model->Validate(*id))
            return {ActionResult::Status::ID_NOT_FOUND, id};
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }

    std::vector<Core::TaskEntity> tasks = model->getTasks(*id);
    tasks.erase(tasks.begin()+1, tasks.end());
    return {ActionResult::Status::SUCCESS, tasks};
}

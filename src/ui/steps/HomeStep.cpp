//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "HomeStep.h"

std::string HomeStep::command() const {
    return command_;
}

std::unique_ptr<Action> HomeStep::genAction(Context &) {
    std::stringstream ss{reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    command_ = command;

    if (command_ == "edit" || command_ == "subtask" ||
        command_ == "delete" || command_ == "complete" ||
        command_ == "uncomplete" || command_ == "label") {
        return std::unique_ptr<Action>(new ValidateIDAction(arg));
    } else if (command_ == "show") {
        return std::unique_ptr<Action>(new GetTasksToShowAction(arg));
    } else if (command_ == "save") {
        return std::unique_ptr<Action>(new SaveToFileAction(arg));
    } else if (command_ == "load") {
        return std::unique_ptr<Action>(new LoadFromFileAction(arg));
    } else {
        return std::unique_ptr<Action>(new ValidateNoArgAction(arg));
    }
}

std::shared_ptr<Step> HomeStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");
}


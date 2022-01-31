//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "HomeStep.h"

HomeStep::HomeStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer) :
        IOStep(reader, printer) {
}

std::string HomeStep::command() const {
    return command_;
}

std::unique_ptr<Action> HomeStep::genAction(Context &) {
    std::stringstream ss{reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    command_ = command;

    std::optional<Core::TaskID> id = stringToID(arg);
    if (command_ == "edit" || command_ == "subtask" ||
        command_ == "delete" || command_ == "complete" ||
        command_ == "uncomplete" || command_ == "label" ||
        command_ == "unlabel" || command_ == "UNLABEL") {
        return std::make_unique<ValidateIDAction>(id);
    } else if (command_ == "show") {
        if (id)
            return std::make_unique<GetTaskToShowByIDAction>(*id);
        else if (arg.empty())
            return std::make_unique<GetAllTasksToShowAction>();
        else
            return std::make_unique<GetTasksToShowByLabelAction>(arg);
    } else if (command_ == "labels") {
        return std::make_unique<GetTaskToShowLabelsAction>(id);
    } else if (command_ == "save") {
        return std::make_unique<SaveToFileAction>(arg);
    } else if (command_ == "load") {
        return std::make_unique<LoadFromFileAction>(arg);
    } else {
        return std::make_unique<ValidateNoArgAction>(arg);
    }
}

std::shared_ptr<Step> HomeStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");
}

std::optional<Core::TaskID> HomeStep::stringToID(const std::string &arg) const {
    Core::TaskID id;
    try {
        id.set_value(std::stoi(arg));
        return id;
    } catch (const std::invalid_argument &) {
        return std::nullopt;
    }
}
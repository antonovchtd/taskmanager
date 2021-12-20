//
// Created by Anton O. on 11/30/21.
//

#include "Action.h"
#include "Context.h"

Action::Action(const std::shared_ptr<TaskManager> &model, const Data &data) :
                model_(model), data_(data) {
    
}

Action::Action(const std::shared_ptr<TaskManager> &model) :
                model_(model) {

}

std::shared_ptr<TaskManager> Action::model() const {
    return model_;
}

Action::Data Action::data() const {
    return data_;
}

void Action::setActionData(const Action::Data &data) {
    data_ = data;
}

ActionResult GetIDAction::make(Context &context) {
    ProtoTask::TaskID id;
    if (data().arg.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    try {
        id.set_num(std::stoi(data().arg));
        return model()->Validate(id);
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::NOT_AN_ID, std::nullopt};
    }
}

ActionResult ValidateNoArgAction::make(Context &context) {
    if (data().arg.empty())
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::TAKES_NO_ARG, std::nullopt};
}

ActionResult ValidateLabelOrIDArgAction::make(Context &context) {
    // empty is OK
    ProtoTask::TaskID id;
    try {
        id.set_num(std::stoi(data().arg));
        return model()->Validate(id);
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}

ActionResult AddTaskAction::make(Context &context) {
    return model()->Add(context.task());
}

ActionResult EditTaskAction::make(Context &context) {
    return model()->Edit(*context.id(), context.task());
}

ActionResult AddSubtaskAction::make(Context &context) {
    return model()->AddSubtask(context.task(), *context.id());
}

ActionResult ShowAction::make(Context &context) {
    if (data().arg.empty())
        context.setTasks(model()->getTasks());
    else if (context.id().has_value() && context.id()->has_num()) {
        context.setTasks(model()->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    }
    else
        context.setTasks(model()->getTasks(data().arg));

    return {ActionResult::Status::SUCCESS, std::nullopt};
}

ActionResult CompleteTaskAction::make(Context &context) {
    return model()->Complete(*context.id());
}

ActionResult DeleteAction::make(Context &context) {
    return model()->Delete(*context.id(), true);
}

ActionResult ConfirmDeleteAction::make(Context &context) {
    try {
        context.setTasks(model()->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, *context.id()};
    }
}

ActionResult LabelAction::make(Context &context) {
    return model()->SetLabel(*context.id(), data().arg);
}

ActionResult SaveAction::make(Context &context) {
    std::string filename = data().arg.empty() ? "data.bin" : data().arg;
    if (Persistor::save(filename, model()))
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::FAILED_TO_OPEN_FILE, std::nullopt};
}

ActionResult LoadAction::make(Context &context) {
    std::string filename = data().arg.empty() ? "data.bin" : data().arg;
    if (Persistor::load(filename, model()))
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::FILE_NOT_FOUND, std::nullopt};
}
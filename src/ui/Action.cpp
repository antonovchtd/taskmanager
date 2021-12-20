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
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::NOT_AN_ID, std::nullopt};
    }
    return {ActionResult::Status::SUCCESS, id};
}

ActionResult ValidateNoArgAction::make(Context &context) {
    if (!data().arg.empty())
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::TAKES_NO_ARG, std::nullopt};
}

ActionResult ValidateLabelOrIDArgAction::make(Context &context) {
    // empty is OK
    ProtoTask::TaskID id;
    try {
        id.set_num(std::stoi(data().arg));
        return {ActionResult::Status::SUCCESS, id};
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}

ActionResult AddTaskAction::make(Context &context) {
    return model()->Add(context.task());
}

void AddSubtaskAction::make(Context &context) {
    context.setID(model()->AddSubtask(context.task(), *context.id()).id);
}

void EditTaskAction::make(Context &context) {
    model()->Edit(*context.id(), context.task());
}

void ShowAction::make(Context &context) {
    if (data().arg.empty())
        context.setTasks(model()->getTasks());
    else if (context.id().has_value() && context.id()->has_num())
        context.setTasks(model()->getTasks(*context.id()));
    else
        context.setTasks(model()->getTasks(data().arg));
}

void CompleteTaskAction::make(Context &context) {
    model()->Complete(*context.id());
}

void DeleteAction::make(Context &context) {
    model()->Delete(*context.id(), true);
}

void ConfirmDeleteAction::make(Context &context) {
    context.setTasks(model()->getTasks(*context.id()));
}

void LabelAction::make(Context &context) {
    model()->SetLabel(*context.id(), data().arg);
}

void SaveAction::make(Context &context) {
    std::string filename = data().arg.empty() ? "data.bin" : data().arg;
    Persistor::save(filename, model());
}

void LoadAction::make(Context &context) {
    std::string filename = data().arg.empty() ? "data.bin" : data().arg;
    Persistor::load(filename, model());
}
//
// Created by Anton O. on 11/30/21.
//

#include "Action.h"

void AddTaskAction::make(TaskManager &model, Context &context) {
    context.setID(model.Add(Task::Create(context.data())));
}

void AddSubtaskAction::make(TaskManager &model, Context &context) {
    context.setID(model.Add(Task::Create(context.data()), context.id().value()));
}

void ValidateIDAction::make(TaskManager &model, Context &context) {
    context.setID(TaskID::Create(context.arg()));
    context.setArg("");
    if (!context.id() || !model.Validate(context.id().value())) {
        context.revertStep();
        context.setID(TaskID::invalidID());
    }
}

void ValidateNoArgAction::make(TaskManager &model, Context &context) {
//    context.setID(TaskID::Create(context.arg()));
//    if (context.id() && context.id().value().isValid()) {
//        context.revertStep();
//        context.setID(std::nullopt);
//    } else
//        context.setID(TaskID::nullid());
    if (!context.arg().empty()){
        context.revertStep();
        context.setID(std::nullopt);
    }
    else
        context.setID(TaskID::nullid());
}

void ValidateLabelAction::make(TaskManager &model, Context &context) {
    context.setID(TaskID::nullid());
//    if (context.arg().empty()) {
//        context.revertStep();
//        Step::print("Command takes a label argument. Try again.\n");
//    }
}

void EditTaskAction::make(TaskManager &model, Context &context) {
    model.Edit(context.id().value(), Task::Create(context.data()));
}

void ShowAction::make(TaskManager &model, Context &context) {
    if (context.arg().empty())
        context.setTasks(model.getTasks());
    else
        context.setTasks(model.getTasks(context.arg()));
}

void CompleteTaskAction::make(TaskManager &model, Context &context) {
    model.Complete(context.id().value());
}

void DeleteTaskAction::make(TaskManager &model, Context &context) {
    model.Delete(context.id().value());
}

void LabelAction::make(TaskManager &model, Context &context) {
    model[context.id().value()].second.SetLabel(context.label());
}
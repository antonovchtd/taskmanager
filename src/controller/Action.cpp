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
    if (!context.arg().empty()){
        context.revertStep();
        context.setID(std::nullopt);
    }
    else
        context.setID(TaskID::nullid());
}

void ValidateLabelArgAction::make(TaskManager &model, Context &context) {
    // context.arg() can be empty, can be something, no check for ID though
    context.setID(TaskID::nullid());
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

void DeleteAction::make(TaskManager &model, Context &context) {
    model.Delete(context.id().value(), true);
}

void ConfirmDeleteAction::make(TaskManager &model, Context &context) {
    if (model[context.id().value()].second.children().empty())
        context.setAskConfirmation(false);
    else
        context.setAskConfirmation(true);
}

void LabelAction::make(TaskManager &model, Context &context) {
    model[context.id().value()].second.SetLabel(context.label());
}
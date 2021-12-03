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
    if (!context.id() || !model.Validate(context.id().value())) {
        context.revertStep();
        context.setID(TaskID::invalidID());
    }
}

void ValidateNoIDAction::make(TaskManager &model, Context &context) {
    if (context.id() && context.id().value().isValid()) {
        context.revertStep();
        context.setID(std::nullopt);
    } else
        context.setID(TaskID::nullid());
}

void EditTaskAction::make(TaskManager &model, Context &context) {
    model.Edit(context.id().value(), Task::Create(context.data()));
}

void ShowAction::make(TaskManager &model, Context &context) {
    context.setTasks(model.getTasks());
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
//
// Created by Anton O. on 11/30/21.
//

#include "Action.h"

Action::Action(const std::shared_ptr<TaskManager> &model) : model_(model) {
    
}

std::shared_ptr<TaskManager> Action::model() const {
    return model_;
}

void AddTaskAction::make(Context &context) {
    context.setID(model()->Add(Task::Create(context.data())));
}

void DoNothingAction::make(Context &context) {
    // do nothing
}

void AddSubtaskAction::make(Context &context) {
    context.setID(model()->AddSubtask(Task::Create(context.data()), *context.id()));
}

void ValidateIDAction::make(Context &context) {
    context.setID(TaskID::Create(context.arg()));
    context.setArg("");
    if (!context.id() || !model()->Validate(context.id().value())) {
        context.setID(TaskID::invalidID());
    }
}

void ValidateNoArgAction::make(Context &context) {
    if (!context.arg().empty()){
        context.setID(std::nullopt);
    }
    else
        context.setID(TaskID::nullid());
}

void ValidateLabelArgAction::make(Context &context) {
    // context.arg() can be empty, can be something, no check for ID though
    TaskID id = TaskID::Create(context.arg());
    if (id.isValid()) {
        if (model()->Validate(id)) {
            context.setID(id);
        }
        else {
            context.setID(TaskID::invalidID());
        }
    }
    else {
        context.setID(TaskID::nullid());
    }
}

void EditTaskAction::make(Context &context) {
    model()->Edit(context.id().value(), Task::Create(context.data()));
}

void ShowAction::make(Context &context) {
    if (context.arg().empty())
        context.setTasks(model()->getTasks());
    else if (context.id().has_value() && context.id()->isValid())
        context.setTasks(model()->getTasks(*context.id()));
    else
        context.setTasks(model()->getTasks(context.arg()));
}

void CompleteTaskAction::make(Context &context) {
    model()->Complete(context.id().value());
}

void DeleteAction::make(Context &context) {
    model()->Delete(context.id().value(), true);
}

void ConfirmDeleteAction::make(Context &context) {
    if ((*model())[context.id().value()].second.children().empty())
        context.setAskConfirmation(false);
    else
        context.setAskConfirmation(true);
}

void LabelAction::make(Context &context) {
    (*model())[context.id().value()].second.SetLabel(context.label());
}
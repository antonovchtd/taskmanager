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
    context.setID(TaskID::Create(data().arg));
    if (!context.id() || !model()->Validate(*context.id())) {
        context.setID(TaskID::invalidID());
    }
}

void ValidateNoArgAction::make(Context &context) {
    if (!data().arg.empty()){
        context.setID(std::nullopt);
    }
    else
        context.setID(TaskID::nullid());
}

void ValidateLabelArgAction::make(Context &context) {
    // arg can be empty, can be something, no check for ID though
    TaskID id = TaskID::Create(data().arg);
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
    if (data().arg.empty())
        context.setTasks(model()->getTasks());
    else if (context.id().has_value() && context.id()->isValid())
        context.setTasks(model()->getTasks(*context.id()));
    else
        context.setTasks(model()->getTasks(data().arg));
}

void CompleteTaskAction::make(Context &context) {
    model()->Complete(context.id().value());
}

void DeleteAction::make(Context &context) {
    model()->Delete(context.id().value(), true);
}

void ConfirmDeleteAction::make(Context &context) {
    context.setTasks(model()->getTasks(*context.id()));
}

void LabelAction::make(Context &context) {
    (*model())[context.id().value()].second.SetLabel(data().arg);
}
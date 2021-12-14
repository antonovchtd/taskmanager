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
    context.setID(model()->Add(context.task()));
}

void DoNothingAction::make(Context &context) {
    // do nothing
}

void AddSubtaskAction::make(Context &context) {
    context.setID(model()->AddSubtask(context.task(), *context.id()));
}

void ValidateIDAction::make(Context &context) {
    ProtoTask::TaskID id;
    try {
        id.set_num(std::stoi(data().arg));
    } catch (const std::invalid_argument &) {
        id.set_is_invalid(true);
    }
    if (!model()->Validate(id)) {
        id.set_is_invalid(true);
    }
    context.setID(id);
}

void ValidateNoArgAction::make(Context &context) {
    if (!data().arg.empty()){
        context.setID(std::nullopt);
    }
    else {
        ProtoTask::TaskID id;
        id.set_is_invalid(false);
        context.setID(id);
    }
}

void ValidateLabelArgAction::make(Context &context) {
    // arg can be empty, can be something, no check for ID though
    ProtoTask::TaskID id;
    try {
        id.set_num(std::stoi(data().arg));
    } catch (const std::invalid_argument &) {
        id.set_is_invalid(false);
    }
    if (id.has_num())
        if (!model()->Validate(id))
            id.set_is_invalid(true);

    context.setID(id);
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
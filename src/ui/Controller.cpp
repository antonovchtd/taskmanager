//
// Created by Anton O. on 11/30/21.
//

#include "Controller.h"
#include "Context.h"

Controller::Controller() : model_{std::make_shared<TaskManager>()}{
};

Controller::Controller(const std::shared_ptr<TaskManager> &model, const Data &data) :
                model_(model), data_(data) {
    
}

Controller::Controller(const std::shared_ptr<TaskManager> &model) :
                model_(model) {

}

std::shared_ptr<TaskManager> Controller::model() const {
    return model_;
}

Controller::Data Controller::data() const {
    return data_;
}

void Controller::setData(const Controller::Data &data) {
    data_ = data;
}

ActionResult Controller::ValidateID(Context &context) {
    ProtoTask::TaskID id;
    if (data().arg.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    try {
        id.set_value(std::stoi(data().arg));
        context.setID(id);
        return model()->Validate(id);
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }
}

ActionResult Controller::ValidateNoArg(Context &context) {
    if (data().arg.empty())
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::TAKES_NO_ARG, std::nullopt};
}

ActionResult Controller::ValidateLabelOrID(Context &context) {
    // empty is OK
    ProtoTask::TaskID id;
    try {
        id.set_value(std::stoi(data().arg));
        context.setID(id);
        return model()->Validate(id);
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}

ActionResult Controller::ValidateAlpha(Context &context) {
    // empty is OK
    ProtoTask::TaskID id;
    try {
        id.set_value(std::stoi(data().arg));
        return {ActionResult::Status::TAKES_ALPHA, id};
    } catch (const std::invalid_argument &) {
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}

ActionResult Controller::AddTask(Context &context) {
    return model()->Add(context.task());
}

ActionResult Controller::EditTask(Context &context) {
    return model()->Edit(*context.id(), context.task());
}

ActionResult Controller::AddSubtask(Context &context) {
    return model()->AddSubtask(context.task(), *context.id());
}

ActionResult Controller::ShowTasks(Context &context) {
    if (data().arg.empty())
        context.setTasks(model()->getTasks());
    else if (context.id()) {
        context.setTasks(model()->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    }
    else
        context.setTasks(model()->getTasks(data().arg));

    return {ActionResult::Status::SUCCESS, std::nullopt};
}

ActionResult Controller::CompleteTask(Context &context) {
    return model()->Complete(*context.id());
}

ActionResult Controller::DeleteTask(Context &context) {
    return model()->Delete(*context.id(), true);
}

ActionResult Controller::ConfirmDeleteTask(Context &context) {
    try {
        context.setTasks(model()->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, *context.id()};
    }
}

ActionResult Controller::LabelTask(Context &context) {
    return model()->SetLabel(*context.id(), data().arg);
}

ActionResult Controller::SaveTasks(Context &context) {
    std::string filename = data().arg.empty() ? filename_ : data().arg;
    if (Persister::save(filename, model()))
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::FAILED_TO_OPEN_FILE, std::nullopt};
}

ActionResult Controller::LoadTasks(Context &context) {
    std::string filename = data().arg.empty() ? filename_ : data().arg;
    if (Persister::load(filename, model()))
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::FILE_NOT_FOUND, std::nullopt};
}
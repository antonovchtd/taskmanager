//
// Created by Anton O. on 11/30/21.
//

#include "Controller.h"
#include "Context.h"

Controller::Controller() : model_{std::unique_ptr<ModelInterface>(new TaskManager)},
                           persister_{nullptr} {
};

Controller::Controller(std::unique_ptr<ModelInterface> model) :
                model_(std::move(model)), persister_{nullptr} {

}

Controller::Data Controller::data() const {
    return data_;
}

void Controller::setData(const Controller::Data &data) {
    data_ = data;
}

ActionResult Controller::ValidateID(Context &context) {
    Core::TaskID id;
    if (data().arg.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    try {
        id.set_value(std::stoi(data().arg));
        context.setID(id);
        return model_->Validate(id);
    } catch (const std::invalid_argument &) {
        context.setID(std::nullopt);
        return {ActionResult::Status::TAKES_ID, std::nullopt};
    }
}

ActionResult Controller::ValidateNoArg(Context &context) {
    if (data().arg.empty())
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::TAKES_NO_ARG, std::nullopt};
}

ActionResult Controller::ValidateAlphaOrID(Context &context) {
    // empty is OK
    Core::TaskID id;
    try {
        id.set_value(std::stoi(data().arg));
        context.setID(id);
        return model_->Validate(id);
    } catch (const std::invalid_argument &) {
        context.setID(std::nullopt);
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}

ActionResult Controller::ValidateAlpha(Context &context) {
    // empty is not OK
    Core::TaskID id;
    try {
        id.set_value(std::stoi(data().arg));
        return {ActionResult::Status::TAKES_ALPHA_NOT_ID, id};
    } catch (const std::invalid_argument &) {
        if (data().arg.empty())
            return {ActionResult::Status::TAKES_ALPHA, std::nullopt};
        else
            return {ActionResult::Status::SUCCESS, std::nullopt};
    }
}

ActionResult Controller::AddTask(Context &context) {
    return model_->Add(context.task());
}

ActionResult Controller::EditTask(Context &context) {
    return model_->Edit(*context.id(), context.task());
}

ActionResult Controller::AddSubtask(Context &context) {
    return model_->AddSubtask(context.task(), *context.id());
}

ActionResult Controller::ShowTasks(Context &context) {
    if (data().arg.empty())
        context.setTasks(model_->getTasks());
    else if (context.id()) {
        context.setTasks(model_->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    }
    else
        context.setTasks(model_->getTasks(data().arg));

    return {ActionResult::Status::SUCCESS, std::nullopt};
}

ActionResult Controller::CompleteTask(Context &context) {
    return model_->Complete(*context.id());
}

ActionResult Controller::UncompleteTask(Context &context) {
    return model_->Uncomplete(*context.id());
}

ActionResult Controller::DeleteTask(Context &context) {
    return model_->Delete(*context.id(), true);
}

ActionResult Controller::ReadTaskWithChildren(Context &context) {
    try {
        context.setTasks(model_->getTasks(*context.id()));
        return {ActionResult::Status::SUCCESS, *context.id()};
    } catch (const std::out_of_range &) {
        return {ActionResult::Status::ID_NOT_FOUND, *context.id()};
    }
}

ActionResult Controller::LabelTask(Context &context) {
    return model_->AddLabel(*context.id(), data().arg);
}

ActionResult Controller::SaveTasks(Context &context) {
    persister_ = std::unique_ptr<Persister>(new FilePersistence{data().arg});
    if (persister_->save(model_->getTasks()))
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::FAILED_TO_OPEN_FILE, std::nullopt};
}

ActionResult Controller::LoadTasks(Context &context) {
    persister_ = std::unique_ptr<Persister>(new FilePersistence{data().arg});
    std::vector<Core::TaskEntity> data;
    if (persister_->load(data)) {
        model_->Replace(data);
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
    else
        return {ActionResult::Status::FILE_NOT_FOUND, std::nullopt};
}
//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include "TaskManagerClient.h"

TaskManagerClient::TaskManagerClient(const std::shared_ptr<grpc::Channel> &channel) :
                                stub_(Transfer::TaskManager::NewStub(channel)) {
}

TaskManagerClient::TaskManagerClient(std::unique_ptr<Transfer::TaskManager::StubInterface> stub) :
                    stub_(std::move(stub)) {
}

std::vector<Core::TaskEntity> ManyTaskEntitiesTransformer(const Transfer::ManyTaskEntities &mte) {
    return std::vector<Core::TaskEntity>(mte.tasks().begin(), mte.tasks().end());
}

std::vector<Core::TaskEntity> TaskManagerClient::getTasks() const {
    Transfer::EmptyMessage request;
    Transfer::ManyTaskEntities reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->getTasks(&context, request, &reply);

    return ManyTaskEntitiesTransformer(reply);
}

std::vector<Core::TaskEntity> TaskManagerClient::getTasks(const std::string &label) const {
    Core::Label request;
    request.set_label(label);
    Transfer::ManyTaskEntities reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->getTasksByLabel(&context, request, &reply);

    return ManyTaskEntitiesTransformer(reply);
}

std::vector<Core::TaskEntity> TaskManagerClient::getTaskWithSubtasks(const Core::TaskID &request) const {
    Transfer::ManyTaskEntities reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->getTaskWithSubtasks(&context, request, &reply);

    return ManyTaskEntitiesTransformer(reply);
}

ActionResult ActionResultTransformer(const Transfer::ActionResult &old_result) {
    ActionResult::Status status;
    switch (old_result.status()) {
        case Transfer::ActionResult_Status_SUCCESS:
            status = ActionResult::Status::SUCCESS;
            break;
        case Transfer::ActionResult_Status_ID_NOT_FOUND:
            status = ActionResult::Status::ID_NOT_FOUND;
            break;
            // TODO
    }
    if (old_result.tasks_size() > 0) {
        std::vector<Core::TaskEntity> vec{old_result.tasks().begin(), old_result.tasks().end()};
        return {status, vec};
    } else if (old_result.has_task()) {
        std::vector<Core::TaskEntity> vec{old_result.task()};
        return {status, vec};
//        ActionResult new_result{status, old_result.task()};
    } else {
        return {status, old_result.id()};
    }
}

ActionResult TaskManagerClient::Add(const Core::Task& request) {
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Add(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::AddSubtask(const Core::Task& task, const Core::TaskID &id) {
    Core::TaskEntity request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_data()->CopyFrom(task);
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->AddSubtask(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::Edit(const Core::TaskID &id, const Core::Task& task) {
    Core::TaskEntity request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_data()->CopyFrom(task);
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Edit(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::Complete(const Core::TaskID &request) {
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Complete(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::Uncomplete(const Core::TaskID &request) {
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Uncomplete(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::Delete(const Core::TaskID &request, bool deleteChildren) {
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Delete(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::IsPresent(const Core::TaskID &request) const {
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->IsPresent(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::AddLabel(const Core::TaskID &id, const std::string &label) {
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_label()->set_label(label);
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->AddLabel(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::RemoveLabel(const Core::TaskID &id, const std::string &label) {
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_label()->set_label(label);
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->ClearLabel(&context, request, &reply);

    return ActionResultTransformer(reply);
}

ActionResult TaskManagerClient::RemoveAllLabels(const Core::TaskID &request) {
    Transfer::ActionResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->ClearLabels(&context, request, &reply);

    return ActionResultTransformer(reply);
}

void TaskManagerClient::Replace(const std::vector<Core::TaskEntity> &vec) {
    Transfer::ManyTaskEntities request;
    request.mutable_tasks()->Add(vec.begin(), vec.end());
    Transfer::EmptyMessage reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Replace(&context, request, &reply);
}

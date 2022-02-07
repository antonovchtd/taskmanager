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

Core::ModelRequestResult TaskManagerClient::Add(const Core::Task& request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Add(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::AddSubtask(const Core::Task& task, const Core::TaskID &id) {
    Core::TaskEntity request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_data()->CopyFrom(task);
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->AddSubtask(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::Edit(const Core::TaskID &id, const Core::Task& task) {
    Core::TaskEntity request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_data()->CopyFrom(task);
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Edit(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::Complete(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Complete(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::Uncomplete(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Uncomplete(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::Delete(const Core::TaskID &request, bool deleteChildren) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Delete(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::IsPresent(const Core::TaskID &request) const {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->IsPresent(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::AddLabel(const Core::TaskID &id, const std::string &label) {
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_label()->set_label(label);
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->AddLabel(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::ClearLabel(const Core::TaskID &id, const std::string &label) {
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id);
    request.mutable_label()->set_label(label);
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->ClearLabel(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerClient::ClearLabels(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->ClearLabels(&context, request, &reply);

    return reply;
}

void TaskManagerClient::Replace(const std::vector<Core::TaskEntity> &vec) {
    Transfer::ManyTaskEntities request;
    request.mutable_tasks()->Add(vec.begin(), vec.end());
    Transfer::EmptyMessage reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Replace(&context, request, &reply);
}

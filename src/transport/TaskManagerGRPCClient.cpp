//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include "TaskManagerGRPCClient.h"

TaskManagerGRPCClient::TaskManagerGRPCClient(const std::shared_ptr<grpc::Channel> &channel) :
                                stub_(Transfer::TaskManager::NewStub(channel)) {
}

TaskManagerGRPCClient::TaskManagerGRPCClient(std::unique_ptr<Transfer::TaskManager::StubInterface> stub) :
                    stub_(std::move(stub)) {
}

std::vector<Core::TaskEntity> ToTaskEntitiesVector(const Transfer::ManyTaskEntities &mte) {
    return std::vector<Core::TaskEntity>(mte.tasks().begin(), mte.tasks().end());
}

std::vector<Core::TaskEntity> TaskManagerGRPCClient::getTasks() {
    Transfer::EmptyMessage request;
    Transfer::ManyTaskEntities reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->getTasks(&context, request, &reply);

    return ToTaskEntitiesVector(reply);
}

std::vector<Core::TaskEntity> TaskManagerGRPCClient::getTasks(const Core::Label &label) {
    Transfer::ManyTaskEntities reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->getTasksByLabel(&context, label, &reply);

    return ToTaskEntitiesVector(reply);
}

std::vector<Core::TaskEntity> TaskManagerGRPCClient::getTaskWithSubtasks(const Core::TaskID &request) {
    Transfer::ManyTaskEntities reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->getTaskWithSubtasks(&context, request, &reply);

    return ToTaskEntitiesVector(reply);
}

Core::ModelRequestResult TaskManagerGRPCClient::Add(const Core::Task& request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Add(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::AddSubtask(const Core::Task& task, const Core::TaskID &id) {
    Core::TaskEntity request;
    request.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    request.set_allocated_data(std::make_unique<Core::Task>(task).release());
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->AddSubtask(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::Edit(const Core::TaskID &id, const Core::Task& task) {
    Core::TaskEntity request;
    request.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    request.set_allocated_data(std::make_unique<Core::Task>(task).release());
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Edit(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::Complete(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Complete(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::Uncomplete(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Uncomplete(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::Delete(const Core::TaskID &request, bool deleteChildren) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Delete(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::CheckTask(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->CheckTask(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::AddLabel(const Core::TaskID &id, const Core::Label &label) {
    Transfer::IDWithLabel request;
    request.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    request.set_allocated_label(std::make_unique<Core::Label>(label).release());
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->AddLabel(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::RemoveLabel(const Core::TaskID &id, const Core::Label &label) {
    Transfer::IDWithLabel request;
    request.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    request.set_allocated_label(std::make_unique<Core::Label>(label).release());
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->RemoveLabel(&context, request, &reply);

    return reply;
}

Core::ModelRequestResult TaskManagerGRPCClient::RemoveAllLabels(const Core::TaskID &request) {
    Core::ModelRequestResult reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->RemoveAllLabels(&context, request, &reply);

    return reply;
}

void TaskManagerGRPCClient::Replace(const std::vector<Core::TaskEntity> &vec) {
    Transfer::ManyTaskEntities request;
    request.mutable_tasks()->Add(vec.begin(), vec.end());
    Transfer::EmptyMessage reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->Replace(&context, request, &reply);
}

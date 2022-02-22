//
// Created by Anton Ovcharenko on 25.01.2022.
//

#include "TaskManagerGRPCService.h"

TaskManagerGRPCService::TaskManagerGRPCService(const std::shared_ptr<ModelInterface> &model) :
                    model_{model} {
}

Transfer::ManyTaskEntities ToManyTaskEntities(const std::vector<Core::TaskEntity> &vec) {
    Transfer::ManyTaskEntities mte;
    mte.mutable_tasks()->Add(vec.begin(), vec.end());
    return mte;
}

grpc::Status TaskManagerGRPCService::getTasks(grpc::ServerContext* context,
                                              const Transfer::EmptyMessage* msg,
                                              Transfer::ManyTaskEntities* result) {
    *result = ToManyTaskEntities(model_->getTasks());
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::getTasksByLabel(grpc::ServerContext* context,
                                                     const Core::Label* label,
                                                     Transfer::ManyTaskEntities* result) {
    *result = ToManyTaskEntities(model_->getTasks(*label));
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::getTaskWithSubtasks(grpc::ServerContext* context,
                                                         const Core::TaskID* id,
                                                         Transfer::ManyTaskEntities* result) {
    *result = ToManyTaskEntities(model_->getTaskWithSubtasks(*id));
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::Add(grpc::ServerContext* context, const Core::Task* task,
                                         Core::ModelRequestResult* result) {
    *result = model_->Add(*task);
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::AddSubtask(grpc::ServerContext* context,
                                                const Core::TaskEntity* te,
                                                Core::ModelRequestResult* result) {
    *result = model_->AddSubtask(te->data(), te->id());
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::Edit(grpc::ServerContext* context,
                                          const Core::TaskEntity* te,
                                          Core::ModelRequestResult* result) {
    *result = model_->Edit(te->id(), te->data());
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::Complete(grpc::ServerContext* context,
                                              const Core::TaskID* id,
                                              Core::ModelRequestResult* result) {
    *result = model_->Complete(*id);
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::Uncomplete(grpc::ServerContext* context,
                                                const Core::TaskID* id,
                                                Core::ModelRequestResult* result) {
    *result = model_->Uncomplete(*id);
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::Delete(grpc::ServerContext* context,
                                            const Core::TaskID* id,
                                            Core::ModelRequestResult* result) {
    *result = model_->Delete(*id, true);
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::CheckTask(grpc::ServerContext* context,
                                               const Core::TaskID* id,
                                               Core::ModelRequestResult* result) {
    *result = model_->CheckTask(*id);
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::AddLabel(grpc::ServerContext* context,
                                              const Transfer::IDWithLabel* msg,
                                              Core::ModelRequestResult* result) {
    *result = model_->AddLabel(msg->id(), msg->label());
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::RemoveLabel(grpc::ServerContext* context,
                                                const Transfer::IDWithLabel* msg,
                                                Core::ModelRequestResult* result) {
    *result = model_->RemoveLabel(msg->id(), msg->label());
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::RemoveAllLabels(grpc::ServerContext* context,
                                                 const Core::TaskID* id,
                                                 Core::ModelRequestResult* result) {
    *result = model_->RemoveAllLabels(*id);
    return grpc::Status::OK;
}

grpc::Status TaskManagerGRPCService::Replace(grpc::ServerContext* context,
                                             const Transfer::ManyTaskEntities* mte,
                                             Transfer::EmptyMessage* result) {
    std::vector<Core::TaskEntity> vec(mte->tasks().begin(), mte->tasks().end());
    model_->Replace(vec);
    return grpc::Status::OK;
}

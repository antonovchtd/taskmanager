//
// Created by Anton Ovcharenko on 25.01.2022.
//

#ifndef TASKMANAGER_SRC_TRANSFER_TASKMANAGERSERVICE_H_
#define TASKMANAGER_SRC_TRANSFER_TASKMANAGERSERVICE_H_

#include "Service.grpc.pb.h"
#include "model/ModelInterface.h"
#include <grpcpp/grpcpp.h>

class TaskManagerGRPCService final : public Transfer::TaskManager::Service {
public:
    explicit TaskManagerGRPCService(const std::shared_ptr<ModelInterface> &model);
    grpc::Status getTasks(grpc::ServerContext* context, const Transfer::EmptyMessage* msg,
                     Transfer::ManyTaskEntities* result) override;
    grpc::Status getTasksByLabel(grpc::ServerContext* context, const Core::Label* label,
                     Transfer::ManyTaskEntities* result) override;
    grpc::Status getTaskWithSubtasks(grpc::ServerContext* context, const Core::TaskID* id,
                     Transfer::ManyTaskEntities* result) override;

public:
    grpc::Status Add(grpc::ServerContext* context, const Core::Task* task,
                     Core::ModelRequestResult* result) override;
    grpc::Status AddSubtask(grpc::ServerContext* context, const Core::TaskEntity* te,
                     Core::ModelRequestResult* result) override;
    grpc::Status Edit(grpc::ServerContext* context, const Core::TaskEntity* te,
                     Core::ModelRequestResult* result) override;
    grpc::Status Complete(grpc::ServerContext* context, const Core::TaskID* id,
                     Core::ModelRequestResult* result) override;
    grpc::Status Uncomplete(grpc::ServerContext* context, const Core::TaskID* id,
                     Core::ModelRequestResult* result) override;
    grpc::Status Delete(grpc::ServerContext* context, const Core::TaskID* id,
                     Core::ModelRequestResult* result) override;
    grpc::Status CheckTask(grpc::ServerContext* context, const Core::TaskID* id,
                           Core::ModelRequestResult* result) override;
    grpc::Status AddLabel(grpc::ServerContext* context, const Transfer::IDWithLabel* msg,
                     Core::ModelRequestResult* result) override;
    grpc::Status RemoveLabel(grpc::ServerContext* context, const Transfer::IDWithLabel* msg,
                     Core::ModelRequestResult* result) override;
    grpc::Status RemoveAllLabels(grpc::ServerContext* context, const Core::TaskID* id,
                     Core::ModelRequestResult* result) override;
public:
    grpc::Status Replace(grpc::ServerContext* context, const Transfer::ManyTaskEntities* mte,
                         Transfer::EmptyMessage* result) override;

private:
    std::shared_ptr<ModelInterface> model_;
};


#endif //TASKMANAGER_SRC_TRANSFER_TASKMANAGERSERVICE_H_

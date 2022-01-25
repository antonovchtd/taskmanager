//
// Created by Anton Ovcharenko on 25.01.2022.
//

#ifndef TASKMANAGER_SRC_TRANSFER_TASKMANAGERSERVICE_H_
#define TASKMANAGER_SRC_TRANSFER_TASKMANAGERSERVICE_H_

#include "Service.grpc.pb.h"
#include "model/ModelInterface.h"
#include <grpcpp/grpcpp.h>

class TaskManagerService final : public Transfer::TaskManager::Service {
public:
    explicit TaskManagerService(const std::shared_ptr<ModelInterface> &model);
    grpc::Status Add(grpc::ServerContext* context, const Core::Task* task,
                     Transfer::ActionResult* result) override;
private:
    std::shared_ptr<ModelInterface> model_;
};


#endif //TASKMANAGER_SRC_TRANSFER_TASKMANAGERSERVICE_H_

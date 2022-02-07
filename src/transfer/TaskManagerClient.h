//
// Created by Anton Ovcharenko on 26.01.2022.
//

#ifndef TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_
#define TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_

#include "Service.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include "model/ModelInterface.h"

class TaskManagerClient : public ModelInterface {
public:
    explicit TaskManagerClient(const std::shared_ptr<grpc::Channel> &channel);
    explicit TaskManagerClient(std::unique_ptr<Transfer::TaskManager::StubInterface> stub);

public:
    std::vector<Core::TaskEntity> getTasks() const override;
    std::vector<Core::TaskEntity> getTasks(const std::string &label) const override;
    std::vector<Core::TaskEntity> getTaskWithSubtasks(const Core::TaskID &id) const override;

public:
    Core::ModelRequestResult Add(const Core::Task &) override;
    Core::ModelRequestResult AddSubtask(const Core::Task &, const Core::TaskID &) override;
    Core::ModelRequestResult Edit(const Core::TaskID &id, const Core::Task &t) override;
    Core::ModelRequestResult Complete(const Core::TaskID &) override;
    Core::ModelRequestResult Uncomplete(const Core::TaskID &) override;
    Core::ModelRequestResult Delete(const Core::TaskID &, bool deleteChildren) override;
    Core::ModelRequestResult IsPresent(const Core::TaskID &) const override;
    Core::ModelRequestResult AddLabel(const Core::TaskID &, const std::string &label) override;
    Core::ModelRequestResult ClearLabel(const Core::TaskID &, const std::string &label) override;
    Core::ModelRequestResult ClearLabels(const Core::TaskID &) override;

public:
    void Replace(const std::vector<Core::TaskEntity> &) override;

private:
    std::unique_ptr<Transfer::TaskManager::StubInterface> stub_;
};


#endif //TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_

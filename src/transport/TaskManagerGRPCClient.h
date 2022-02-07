//
// Created by Anton Ovcharenko on 26.01.2022.
//

#ifndef TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_
#define TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_

#include "Service.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include "model/ModelInterface.h"

class TaskManagerGRPCClient : public ModelInterface {
public:
    explicit TaskManagerGRPCClient(const std::shared_ptr<grpc::Channel> &channel);
    explicit TaskManagerGRPCClient(std::unique_ptr<Transfer::TaskManager::StubInterface> stub);

public:
    std::vector<Core::TaskEntity> getTasks() const override;
    std::vector<Core::TaskEntity> getTasks(const std::string &label) const override;
    std::vector<Core::TaskEntity> getTaskWithSubtasks(const Core::TaskID &id) const override;

public:
    ActionResult Add(const Core::Task &) override;
    ActionResult AddSubtask(const Core::Task &, const Core::TaskID &) override;
    ActionResult Edit(const Core::TaskID &id, const Core::Task &t) override;
    ActionResult Complete(const Core::TaskID &) override;
    ActionResult Uncomplete(const Core::TaskID &) override;
    ActionResult Delete(const Core::TaskID &, bool deleteChildren) override;
    ActionResult IsPresent(const Core::TaskID &) const override;
    ActionResult AddLabel(const Core::TaskID &, const std::string &label) override;
    ActionResult RemoveLabel(const Core::TaskID &, const std::string &label) override;
    ActionResult RemoveAllLabels(const Core::TaskID &) override;

public:
    void Replace(const std::vector<Core::TaskEntity> &) override;

private:
    std::unique_ptr<Transfer::TaskManager::StubInterface> stub_;
};


#endif //TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_
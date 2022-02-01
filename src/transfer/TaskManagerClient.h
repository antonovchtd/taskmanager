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
    Core::ModelInquiryResult Add(const Core::Task &) override;
    Core::ModelInquiryResult AddSubtask(const Core::Task &, const Core::TaskID &) override;
    Core::ModelInquiryResult Edit(const Core::TaskID &id, const Core::Task &t) override;
    Core::ModelInquiryResult Complete(const Core::TaskID &) override;
    Core::ModelInquiryResult Uncomplete(const Core::TaskID &) override;
    Core::ModelInquiryResult Delete(const Core::TaskID &, bool deleteChildren) override;
    Core::ModelInquiryResult IsPresent(const Core::TaskID &) const override;
    Core::ModelInquiryResult AddLabel(const Core::TaskID &, const std::string &label) override;
    Core::ModelInquiryResult ClearLabel(const Core::TaskID &, const std::string &label) override;
    Core::ModelInquiryResult ClearLabels(const Core::TaskID &) override;

public:
    void Replace(const std::vector<Core::TaskEntity> &) override;

private:
    std::unique_ptr<Transfer::TaskManager::StubInterface> stub_;
};


#endif //TASKMANAGER_SRC_TRANSFER_TASKMANAGERCLIENT_H_

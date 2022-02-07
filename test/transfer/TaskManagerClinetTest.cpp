//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include "transfer/TaskManagerClient.h"
#include "Service_mock.grpc.pb.h"
#include "utilities/TaskUtils.h"
#include "utilities/TaskEntityUtils.h"
#include "utilities/TaskIDUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;
using ::grpc::ClientContext;
using ::Transfer::EmptyMessage;
using ::Transfer::ManyTaskEntities;
using ::Core::Label;
using ::Core::TaskID;
using ::Core::Task;
using ::Core::TaskEntity;
using ::Transfer::IDWithLabel;
using ::Transfer::MockTaskManagerStub;

class TaskManagerClientTest : public ::testing::Test
{
public:
    Core::Task task_;
    Core::TaskID id_;
    Core::TaskEntity entity_;

    void SetUp() override {
        task_.set_title("test");
        task_.set_priority(Core::Task::Priority::Task_Priority_HIGH);
        task_.set_due_date(time(nullptr));
        task_.add_labels("label");
        task_.set_is_complete(false);
        id_.set_value(42);
        entity_.mutable_id()->CopyFrom(id_);
        entity_.mutable_data()->CopyFrom(task_);
    }
};

namespace Core {
bool operator==(const Core::Label &lhs, const Core::Label &rhs) {
    return rhs.label() == lhs.label();
}
}

namespace Transfer {
bool operator==(const Transfer::IDWithLabel &lhs, const Transfer::IDWithLabel &rhs) {
    return rhs.label() == lhs.label() &&
    rhs.id() == lhs.id();
}

bool operator==(const Transfer::ManyTaskEntities &lhs, const Transfer::ManyTaskEntities &rhs) {
    bool ans = rhs.tasks_size() == lhs.tasks_size();
    if (ans) {
        for (int i = 0; i < rhs.tasks_size(); ++i) {
            ans = rhs.tasks(i) == lhs.tasks(i);
            if (!ans)
                return ans;
        }
    }

    return ans;
}
}

TEST_F(TaskManagerClientTest, shouldSendGetTasksRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    EXPECT_CALL(*stub, getTasks(_, _, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const EmptyMessage &request, ManyTaskEntities* reply) -> grpc::Status {
                    reply->mutable_tasks()->AddAllocated(new TaskEntity(entity_));
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    std::vector<TaskEntity> result = client.getTasks();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], entity_);
}

TEST_F(TaskManagerClientTest, shouldSendGetTasksByLabelRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    Core::Label label;
    std::string request_label = "request_label";
    label.set_label(request_label);
    entity_.mutable_data()->add_labels(request_label);

    EXPECT_CALL(*stub, getTasksByLabel(_, label, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const Core::Label &request, ManyTaskEntities* reply) -> grpc::Status {
                    reply->mutable_tasks()->AddAllocated(new TaskEntity(entity_));
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    std::vector<TaskEntity> result = client.getTasks(request_label);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], entity_);
}

TEST_F(TaskManagerClientTest, shouldSendgetTaskWithSubtasksRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, getTaskWithSubtasks(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, ManyTaskEntities* reply) -> grpc::Status {
                    reply->mutable_tasks()->AddAllocated(new TaskEntity(entity_));
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    std::vector<TaskEntity> result = client.getTaskWithSubtasks(id_);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], entity_);
}

TEST_F(TaskManagerClientTest, shouldSendAddRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    EXPECT_CALL(*stub, Add(_, task_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const Task &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Add(task_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendAddSubtaskRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    TaskID subtask_id;
    subtask_id.set_value(4242);

    EXPECT_CALL(*stub, AddSubtask(_, entity_, _))
        .WillOnce(testing::Invoke(
                [subtask_id] (ClientContext*, const TaskEntity &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(subtask_id).release());
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.AddSubtask(task_, id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), subtask_id);
}

TEST_F(TaskManagerClientTest, shouldSendEditRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Edit(_, entity_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskEntity &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(entity_.id()).release());
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Edit(id_, task_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendCompleteRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Complete(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Complete(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendUncompleteRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Uncomplete(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Uncomplete(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendDeleteRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Delete(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Delete(id_, true);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendIsPresentRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, IsPresent(_, id_, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.IsPresent(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendAddLabelRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request.mutable_label()->set_label(new_label);

    EXPECT_CALL(*stub, AddLabel(_, request, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const IDWithLabel &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.AddLabel(id_, new_label);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendRemoveLabelRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string label = "label";
    request.mutable_label()->set_label(label);

    EXPECT_CALL(*stub, ClearLabel(_, request, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const IDWithLabel &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.ClearLabel(id_, label);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendRemoveAllLabelsRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, ClearLabels(_, id_, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerClient client{std::move(stub)};
    Core::ModelRequestResult result = client.ClearLabels(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerClientTest, shouldSendReplaceRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    ManyTaskEntities mte;
    mte.mutable_tasks()->AddAllocated(new TaskEntity(entity_));

    EXPECT_CALL(*stub, Replace(_, mte, _))
            .WillOnce(testing::Invoke(
                    [] (ClientContext*, const ManyTaskEntities &request, EmptyMessage* reply) -> grpc::Status {
                        return grpc::Status::OK;
                    }));

    TaskManagerClient client{std::move(stub)};
    std::vector<TaskEntity> vec{entity_};
    client.Replace(vec);
}

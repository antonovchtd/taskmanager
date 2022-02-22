//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include "transport/TaskManagerGRPCClient.h"
#include "Service_mock.grpc.pb.h"
#include "utilities/TaskUtils.h"
#include "utilities/TaskEntityUtils.h"
#include "utilities/TaskIDUtils.h"
#include "utilities/LabelUtils.h"

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

class TaskManagerGRPCClientTest : public ::testing::Test
{
public:
    Core::Task task_;
    Core::TaskID id_;
    Core::TaskEntity entity_;

    void SetUp() override {
        task_ = Core::createTask("test",
                                 Core::Task::Priority::Task_Priority_HIGH,
                                 time(nullptr),
                                 "label",
                                 false);
        task_.set_is_complete(false);
        id_.set_value(42);
        entity_.set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
        entity_.set_allocated_data(std::make_unique<Core::Task>(task_).release());
    }
};

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

TEST_F(TaskManagerGRPCClientTest, shouldSendGetTasksRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    EXPECT_CALL(*stub, getTasks(_, _, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const EmptyMessage &request, ManyTaskEntities* reply) -> grpc::Status {
                    reply->mutable_tasks()->AddAllocated(new TaskEntity(entity_));
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    std::vector<TaskEntity> result = client.getTasks();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], entity_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendGetTasksByLabelRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    Core::Label label;
    std::string request_label = "request_label";
    label.set_str(request_label);
    entity_.mutable_data()->mutable_labels()->AddAllocated(std::make_unique<Core::Label>(label).release());

    EXPECT_CALL(*stub, getTasksByLabel(_, label, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const Core::Label &request, ManyTaskEntities* reply) -> grpc::Status {
                    reply->mutable_tasks()->AddAllocated(new TaskEntity(entity_));
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    std::vector<TaskEntity> result = client.getTasks(label);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], entity_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendgetTaskWithSubtasksRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, getTaskWithSubtasks(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, ManyTaskEntities* reply) -> grpc::Status {
                    reply->mutable_tasks()->AddAllocated(new TaskEntity(entity_));
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    std::vector<TaskEntity> result = client.getTaskWithSubtasks(id_);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], entity_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendAddRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    EXPECT_CALL(*stub, Add(_, task_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const Task &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Add(task_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendAddSubtaskRequest)
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

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.AddSubtask(task_, id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), subtask_id);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendEditRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Edit(_, entity_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskEntity &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(entity_.id()).release());
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Edit(id_, task_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendCompleteRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Complete(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Complete(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendUncompleteRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Uncomplete(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Uncomplete(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendDeleteRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, Delete(_, id_, _))
        .WillOnce(testing::Invoke(
                [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                    reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                    return grpc::Status::OK;
                }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.Delete(id_, true);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendIsPresentRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, CheckTask(_, id_, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.CheckTask(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendAddLabelRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    IDWithLabel request;
    request.set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
    Core::Label new_label;
    new_label.set_str("new_label");
    request.set_allocated_label(std::make_unique<Core::Label>(new_label).release());

    EXPECT_CALL(*stub, AddLabel(_, request, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const IDWithLabel &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.AddLabel(id_, new_label);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendRemoveLabelRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    IDWithLabel request;
    request.set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
    Core::Label label;
    label.set_str("label");
    request.set_allocated_label(std::make_unique<Core::Label>(label).release());

    EXPECT_CALL(*stub, RemoveLabel(_, request, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const IDWithLabel &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.RemoveLabel(id_, label);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendRemoveAllLabelsRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();

    EXPECT_CALL(*stub, RemoveAllLabels(_, id_, _))
            .WillOnce(testing::Invoke(
                    [this] (ClientContext*, const TaskID &request, Core::ModelRequestResult* reply) -> grpc::Status {
                        reply->set_allocated_id(std::make_unique<Core::TaskID>(id_).release());
                        return grpc::Status::OK;
                    }));

    TaskManagerGRPCClient client{std::move(stub)};
    Core::ModelRequestResult result = client.RemoveAllLabels(id_);
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(result.id(), id_);
}

TEST_F(TaskManagerGRPCClientTest, shouldSendReplaceRequest)
{
    auto stub = std::make_unique<MockTaskManagerStub>();
    ManyTaskEntities mte;
    mte.mutable_tasks()->AddAllocated(new TaskEntity(entity_));

    EXPECT_CALL(*stub, Replace(_, mte, _))
            .WillOnce(testing::Invoke(
                    [] (ClientContext*, const ManyTaskEntities &request, EmptyMessage* reply) -> grpc::Status {
                        return grpc::Status::OK;
                    }));

    TaskManagerGRPCClient client{std::move(stub)};
    std::vector<TaskEntity> vec{entity_};
    client.Replace(vec);
}

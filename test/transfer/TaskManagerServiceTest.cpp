//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include "transport/TaskManagerGRPCService.h"
#include "model/TaskManager.h"
#include "ui/actions/AddTaskAction.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class TaskManagerGRPCServiceTest : public ::testing::Test
{
public:
    std::shared_ptr<ModelInterface> tm_;
    Core::TaskID id_;
    Core::Task task_;
    std::shared_ptr<TaskManagerGRPCService> service_;

    void SetUp() override {
        tm_ = std::shared_ptr<ModelInterface>(new TaskManager);
        task_ = Core::createTask("test",
                                 Core::Task::Priority::Task_Priority_HIGH,
                                 time(nullptr),
                                 "label",
                                 false);
        AddTaskAction act{task_};
        ActionResult result = act.execute(tm_);
        id_ = result.model_result->id();
        service_ = std::make_shared<TaskManagerGRPCService>(tm_);
    }
};

TEST_F(TaskManagerGRPCServiceTest, shouldGetAllTasks)
{
    grpc::ServerContext context;
    Transfer::EmptyMessage msg;
    Transfer::ManyTaskEntities result;
    grpc::Status status = service_->getTasks(&context, &msg, &result);
    ASSERT_TRUE(status.ok());
    auto tasks = result.tasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ(tasks[0].id(), id_);
    EXPECT_EQ(tasks[0].data(), task_);
    EXPECT_FALSE(tasks[0].has_parent());
}

TEST_F(TaskManagerGRPCServiceTest, shouldGetTaskByLabel)
{
    grpc::ServerContext context;
    Core::Label request;
    request.set_str("label");
    Transfer::ManyTaskEntities result;
    grpc::Status status = service_->getTasksByLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    auto tasks = result.tasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ(tasks[0].id(), id_);
    EXPECT_EQ(tasks[0].data(), task_);
    EXPECT_FALSE(tasks[0].has_parent());
}

TEST_F(TaskManagerGRPCServiceTest, shouldGetTaskByID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Transfer::ManyTaskEntities result;
    grpc::Status status = service_->getTaskWithSubtasks(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    auto tasks = result.tasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ(tasks[0].id(), id_);
    EXPECT_EQ(tasks[0].data(), task_);
    EXPECT_FALSE(tasks[0].has_parent());
}

TEST_F(TaskManagerGRPCServiceTest, shouldAddTask)
{
    grpc::ServerContext context;
    Core::Task request;
    request.CopyFrom(task_);
    Core::ModelRequestResult result;
    grpc::Status status = service_->Add(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_.value()+1, result.id().value());
    EXPECT_EQ(2, tm_->getTasks().size());
}

TEST_F(TaskManagerGRPCServiceTest, shouldAddSubtask)
{
    grpc::ServerContext context;
    Core::TaskEntity request;
    request.mutable_data()->CopyFrom(task_);
    request.mutable_id()->CopyFrom(id_);
    Core::ModelRequestResult result;
    grpc::Status status = service_->AddSubtask(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_.value()+1, result.id().value());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(2, tasks.size());
    ASSERT_TRUE(tasks[1].has_parent());
    EXPECT_EQ(id_, tasks[1].parent());
    EXPECT_EQ(result.id(), tasks[1].id());
}

TEST_F(TaskManagerGRPCServiceTest, shouldEditTask)
{
    grpc::ServerContext context;
    Core::TaskEntity request;
    request.mutable_data()->CopyFrom(task_);
    std::string new_title = "edited";
    request.mutable_data()->set_title(new_title);
    request.mutable_id()->CopyFrom(id_);
    Core::ModelRequestResult result;
    grpc::Status status = service_->Edit(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ(new_title, tasks[0].data().title());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToEditTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskEntity request;
    request.mutable_data()->CopyFrom(task_);
    std::string new_title = "edited";
    request.mutable_data()->set_title(new_title);
    request.mutable_id()->set_value(id_.value()+1);
    Core::ModelRequestResult result;
    grpc::Status status = service_->Edit(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());
}

TEST_F(TaskManagerGRPCServiceTest, shouldCompleteTask)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Core::ModelRequestResult result;
    grpc::Status status = service_->Complete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_TRUE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToCompleteTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value()+1);
    Core::ModelRequestResult result;
    grpc::Status status = service_->Complete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_FALSE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerGRPCServiceTest, shouldUncompleteTask)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Core::ModelRequestResult result;
    service_->Complete(&context, &request, &result);
    grpc::Status status = service_->Uncomplete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_FALSE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToUncompleteTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value());
    Core::ModelRequestResult result;
    service_->Complete(&context, &request, &result);

    request.set_value(id_.value()+1);
    grpc::Status status = service_->Uncomplete(&context, &request, &result);

    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_TRUE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerGRPCServiceTest, shouldDeleteTaskWithSubtasks)
{
    grpc::ServerContext context;
    Core::TaskEntity request1;
    request1.mutable_id()->CopyFrom(id_);
    request1.mutable_data()->CopyFrom(task_);
    Core::ModelRequestResult result;
    service_->AddSubtask(&context, &request1, &result);

    Core::TaskID request2;
    request2.CopyFrom(id_);
    grpc::Status status = service_->Delete(&context, &request2, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(0, tasks.size());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailDeleteTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value()+1);
    Core::ModelRequestResult result;
    grpc::Status status = service_->Delete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
}

TEST_F(TaskManagerGRPCServiceTest, shouldReturnSuccessOnIsPresent)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Core::ModelRequestResult result;

    grpc::Status status = service_->CheckTask(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
}

TEST_F(TaskManagerGRPCServiceTest, shouldReturnIDNotFoundOnIsPresent)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value()+1);
    Core::ModelRequestResult result;
    grpc::Status status = service_->CheckTask(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());
}

TEST_F(TaskManagerGRPCServiceTest, shouldAddLabel)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request.mutable_label()->set_str(new_label);
    Core::ModelRequestResult result;

    grpc::Status status = service_->AddLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(2, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
    EXPECT_EQ(new_label, labels[1].str());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToAddLabelWithInvalidID)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->set_value(id_.value()+1);
    std::string new_label = "new_label";
    request.mutable_label()->set_str(new_label);
    Core::ModelRequestResult result;

    grpc::Status status = service_->AddLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(1, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
}

TEST_F(TaskManagerGRPCServiceTest, shouldRemoveLabel)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    auto label_to_remove = task_.labels(0);
    request.mutable_label()->CopyFrom(label_to_remove);
    Core::ModelRequestResult result;

    grpc::Status status = service_->RemoveLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(0, labels.size());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToRemoveLabelWithInvalidID)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->set_value(id_.value()+1);
    auto label_to_remove = task_.labels(0);
    request.mutable_label()->CopyFrom(label_to_remove);
    Core::ModelRequestResult result;

    grpc::Status status = service_->RemoveLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(1, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToRemoveLabelWrongLabel)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string label_to_remove = "missing";
    request.mutable_label()->set_str(label_to_remove);
    Core::ModelRequestResult result;

    grpc::Status status = service_->RemoveLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_FALSE(result.has_status());
    ASSERT_FALSE(result.has_id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(1, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
}

TEST_F(TaskManagerGRPCServiceTest, shouldRemoveAllLabels)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request1;
    request1.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request1.mutable_label()->set_str(new_label);
    Core::ModelRequestResult result;

    service_->AddLabel(&context, &request1, &result);
    ASSERT_EQ(2, tm_->getTasks()[0].data().labels().size());

    Core::TaskID request2;
    request2.CopyFrom(id_);

    grpc::Status status = service_->RemoveAllLabels(&context, &request2, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(0, labels.size());
}

TEST_F(TaskManagerGRPCServiceTest, shouldFailToRemoveAllLabelsWithWrongID)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request1;
    request1.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request1.mutable_label()->set_str(new_label);
    Core::ModelRequestResult result;

    service_->AddLabel(&context, &request1, &result);
    ASSERT_EQ(2, tm_->getTasks()[0].data().labels().size());

    Core::TaskID request2;
    request2.set_value(id_.value()+1);

    grpc::Status status = service_->RemoveAllLabels(&context, &request2, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelRequestResult_Status_ID_NOT_FOUND, result.status());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(2, labels.size());
}

TEST_F(TaskManagerGRPCServiceTest, shouldReplaceAllTasks)
{
    Core::TaskEntity te;
    te.mutable_data()->CopyFrom(task_);
    std::string new_title = "new";
    te.mutable_data()->set_title(new_title);
    te.mutable_id()->CopyFrom(id_);

    Transfer::ManyTaskEntities request;
    request.mutable_tasks()->Add(std::move(te));

    grpc::ServerContext context;
    Transfer::EmptyMessage result;

    grpc::Status status = service_->Replace(&context, &request, &result);
    ASSERT_TRUE(status.ok());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(request.tasks().size(), tasks.size());
    EXPECT_EQ(new_title, tasks[0].data().title());
}
//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include "transfer/TaskManagerService.h"
#include "model/TaskManager.h"
#include "ui/actions/AddTaskAction.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class TaskManagerServiceTest : public ::testing::Test
{
public:
    std::shared_ptr<ModelInterface> tm_;
    Core::TaskID id_;
    Core::Task task_;
    std::shared_ptr<TaskManagerService> service_;

    void SetUp() override {
        tm_ = std::shared_ptr<ModelInterface>(new TaskManager);
        task_.set_title("test");
        task_.set_priority(Core::Task::Priority::Task_Priority_HIGH);
        task_.set_due_date(time(nullptr));
        task_.add_labels("label");
        task_.set_is_complete(false);
        AddTaskAction act{task_};
        ActionResult result = act.execute(tm_);
        id_ = result.model_result->id();
        service_ = std::make_shared<TaskManagerService>(tm_);
    }
};

TEST_F(TaskManagerServiceTest, shouldGetAllTasks)
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

TEST_F(TaskManagerServiceTest, shouldGetTaskByLabel)
{
    grpc::ServerContext context;
    Core::Label request;
    request.set_label("label");
    Transfer::ManyTaskEntities result;
    grpc::Status status = service_->getTasksByLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    auto tasks = result.tasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ(tasks[0].id(), id_);
    EXPECT_EQ(tasks[0].data(), task_);
    EXPECT_FALSE(tasks[0].has_parent());
}

TEST_F(TaskManagerServiceTest, shouldGetTaskByID)
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

TEST_F(TaskManagerServiceTest, shouldAddTask)
{
    grpc::ServerContext context;
    Core::Task request;
    request.CopyFrom(task_);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->Add(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_.value()+1, result.id().value());
    EXPECT_EQ(2, tm_->getTasks().size());
}

TEST_F(TaskManagerServiceTest, shouldAddSubtask)
{
    grpc::ServerContext context;
    Core::TaskEntity request;
    request.mutable_data()->CopyFrom(task_);
    request.mutable_id()->CopyFrom(id_);
    Core::ModelInquiryResult result;
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

TEST_F(TaskManagerServiceTest, shouldEditTask)
{
    grpc::ServerContext context;
    Core::TaskEntity request;
    request.mutable_data()->CopyFrom(task_);
    std::string new_title = "edited";
    request.mutable_data()->set_title(new_title);
    request.mutable_id()->CopyFrom(id_);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->Edit(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_EQ(new_title, tasks[0].data().title());
}

TEST_F(TaskManagerServiceTest, shouldFailToEditTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskEntity request;
    request.mutable_data()->CopyFrom(task_);
    std::string new_title = "edited";
    request.mutable_data()->set_title(new_title);
    request.mutable_id()->set_value(id_.value()+1);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->Edit(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());
}

TEST_F(TaskManagerServiceTest, shouldCompleteTask)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->Complete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_TRUE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerServiceTest, shouldFailToCompleteTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value()+1);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->Complete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_FALSE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerServiceTest, shouldUncompleteTask)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Core::ModelInquiryResult result;
    service_->Complete(&context, &request, &result);
    grpc::Status status = service_->Uncomplete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_FALSE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerServiceTest, shouldFailToUncompleteTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value());
    Core::ModelInquiryResult result;
    service_->Complete(&context, &request, &result);

    request.set_value(id_.value()+1);
    grpc::Status status = service_->Uncomplete(&context, &request, &result);

    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
    EXPECT_TRUE(tasks[0].data().is_complete());
}

TEST_F(TaskManagerServiceTest, shouldDeleteTaskWithSubtasks)
{
    grpc::ServerContext context;
    Core::TaskEntity request1;
    request1.mutable_id()->CopyFrom(id_);
    request1.mutable_data()->CopyFrom(task_);
    Core::ModelInquiryResult result;
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

TEST_F(TaskManagerServiceTest, shouldFailDeleteTaskWithWrongID)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value()+1);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->Delete(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());
    auto tasks = tm_->getTasks();
    EXPECT_EQ(1, tasks.size());
}

TEST_F(TaskManagerServiceTest, shouldReturnSuccessOnIsPresent)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.CopyFrom(id_);
    Core::ModelInquiryResult result;

    grpc::Status status = service_->IsPresent(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());
}

TEST_F(TaskManagerServiceTest, shouldReturnIDNotFoundOnIsPresent)
{
    grpc::ServerContext context;
    Core::TaskID request;
    request.set_value(id_.value()+1);
    Core::ModelInquiryResult result;
    grpc::Status status = service_->IsPresent(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());
}

TEST_F(TaskManagerServiceTest, shouldAddLabel)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request.mutable_label()->set_label(new_label);
    Core::ModelInquiryResult result;

    grpc::Status status = service_->AddLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(2, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
    EXPECT_EQ(new_label, labels[1]);
}

TEST_F(TaskManagerServiceTest, shouldFailToAddLabelWithInvalidID)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->set_value(id_.value()+1);
    std::string new_label = "new_label";
    request.mutable_label()->set_label(new_label);
    Core::ModelInquiryResult result;

    grpc::Status status = service_->AddLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(1, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
}

TEST_F(TaskManagerServiceTest, shouldRemoveLabel)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string label_to_remove = task_.labels(0);
    request.mutable_label()->set_label(label_to_remove);
    Core::ModelInquiryResult result;

    grpc::Status status = service_->ClearLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(0, labels.size());
}

TEST_F(TaskManagerServiceTest, shouldFailToRemoveLabelWithInvalidID)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->set_value(id_.value()+1);
    std::string label_to_remove = task_.labels(0);
    request.mutable_label()->set_label(label_to_remove);
    Core::ModelInquiryResult result;

    grpc::Status status = service_->ClearLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(1, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
}

TEST_F(TaskManagerServiceTest, shouldFailToRemoveLabelWrongLabel)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request;
    request.mutable_id()->CopyFrom(id_);
    std::string label_to_remove = "missing";
    request.mutable_label()->set_label(label_to_remove);
    Core::ModelInquiryResult result;

    grpc::Status status = service_->ClearLabel(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(1, labels.size());
    EXPECT_EQ(task_.labels(0), labels[0]);
}

TEST_F(TaskManagerServiceTest, shouldRemoveAllLabels)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request1;
    request1.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request1.mutable_label()->set_label(new_label);
    Core::ModelInquiryResult result;

    service_->AddLabel(&context, &request1, &result);
    ASSERT_EQ(2, tm_->getTasks()[0].data().labels().size());

    Core::TaskID request2;
    request2.CopyFrom(id_);

    grpc::Status status = service_->ClearLabels(&context, &request2, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(id_, result.id());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(0, labels.size());
}

TEST_F(TaskManagerServiceTest, shouldFailToRemoveAllLabelsWithWrongID)
{
    grpc::ServerContext context;
    Transfer::IDWithLabel request1;
    request1.mutable_id()->CopyFrom(id_);
    std::string new_label = "new_label";
    request1.mutable_label()->set_label(new_label);
    Core::ModelInquiryResult result;

    service_->AddLabel(&context, &request1, &result);
    ASSERT_EQ(2, tm_->getTasks()[0].data().labels().size());

    Core::TaskID request2;
    request2.set_value(id_.value()+1);

    grpc::Status status = service_->ClearLabels(&context, &request2, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(Core::ModelInquiryResult_Status_ID_NOT_FOUND, result.status());

    auto tasks = tm_->getTasks();
    ASSERT_EQ(1, tasks.size());
    auto labels = tasks[0].data().labels();
    ASSERT_EQ(2, labels.size());
}

TEST_F(TaskManagerServiceTest, shouldReplaceAllTasks)
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
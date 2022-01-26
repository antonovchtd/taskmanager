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
        id_ = *result.id;
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
    Transfer::ActionResult result;
    grpc::Status status = service_->Add(&context, &request, &result);
    ASSERT_TRUE(status.ok());
    ASSERT_TRUE(result.has_id());
    EXPECT_EQ(Transfer::ActionResult_Status_SUCCESS, result.status());
    EXPECT_EQ(id_.value()+1, result.id().value());
    EXPECT_EQ(2, tm_->getTasks().size());
}

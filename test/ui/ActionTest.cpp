//
// Created by Anton O. on 12/5/21.
//

#include "persistence/FilePersistence.h"
#include "utilities/TaskEntityUtils.h"
#include "ui/Context.h"
#include "ui/actions/Action.h"
#include "ui/actions/AddTaskAction.h"
#include "ui/actions/AddSubtaskAction.h"
#include "ui/actions/ValidateIDAction.h"
#include "ui/actions/ValidateNoArgAction.h"
#include "ui/actions/EditTaskAction.h"
#include "ui/actions/GetTasksToShowAction.h"
#include "ui/actions/CompleteTaskAction.h"
#include "ui/actions/UncompleteTaskAction.h"
#include "ui/actions/DeleteTaskAction.h"
#include "ui/actions/DoNothingAction.h"
#include "ui/actions/LabelTaskAction.h"
#include "ui/actions/LoadFromFileAction.h"
#include "ui/actions/SaveToFileAction.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;
using ::testing::DoAll;

class ActionTest : public ::testing::Test
{
public:
    std::shared_ptr<ModelInterface> tm_;
    Core::TaskID id_;
    Core::Task task_;

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
    }

};

TEST_F(ActionTest, shouldAddTask)
{
    ASSERT_EQ(1, tm_->size());
    EXPECT_TRUE(tm_->Validate(id_));
}

TEST_F(ActionTest, shouldAddSubtask)
{
    auto t = task_;
    t.set_title("sub");
    t.set_priority(Core::Task::Priority::Task_Priority_MEDIUM);
    AddSubtaskAction subact{id_, t};
    ActionResult result_subtask = subact.execute(tm_);
    ASSERT_EQ(2, tm_->size());
    EXPECT_TRUE(tm_->Validate(*result_subtask.id));
    EXPECT_NE(id_, *result_subtask.id);

    auto tasks = tm_->getTasks();
    EXPECT_EQ(id_, tasks[1].parent());
}

TEST_F(ActionTest, shouldExecuteValidateIDValidID)
{
    ValidateIDAction act{std::to_string(id_.value())};
    ActionResult result_validate = act.execute(tm_);
    ASSERT_EQ(result_validate.status, ActionResult::Status::SUCCESS);
    EXPECT_TRUE(result_validate);
    EXPECT_EQ(task_, result_validate.tasks[0].data());
}

TEST_F(ActionTest, shouldExecuteValidateIDInvalidID)
{
    ValidateIDAction act{std::to_string(id_.value()) + "0"};
    ActionResult result_validate = act.execute(tm_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_FALSE(result_validate);
    EXPECT_EQ(id_.value(), result_validate.id->value()/10);
}

TEST_F(ActionTest, shouldExecuteValidateIDNoID)
{
    ValidateIDAction act{""};
    ActionResult result_validate = act.execute(tm_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ARG);
    EXPECT_FALSE(result_validate);
    EXPECT_FALSE(result_validate.id.has_value());
}

TEST_F(ActionTest, shouldExecuteValidateIDBadString)
{
    ValidateIDAction act{"bad"};
    ActionResult result_validate = act.execute(tm_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ID);
    EXPECT_FALSE(result_validate);
    EXPECT_FALSE(result_validate.id.has_value());
}

TEST_F(ActionTest, shouldExecuteValidateNoArgActionSomeArg)
{
    ValidateNoArgAction act{"1"};
    ActionResult result = act.execute(tm_);
    EXPECT_EQ(result.status, ActionResult::Status::TAKES_NO_ARG);
    EXPECT_FALSE(result.id.has_value());
}

TEST_F(ActionTest, shouldExecuteValidateNoArgActionNoArg)
{
    ValidateNoArgAction act{""};
    ActionResult result = act.execute(tm_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_FALSE(result.id.has_value());
}

TEST_F(ActionTest, shouldEditTask)
{
    auto t = task_;
    std::string new_title = "edited";
    t.set_title(new_title);
    Core::Task::Priority new_priority = Core::Task::Priority::Task_Priority_MEDIUM;
    t.set_priority(new_priority);
    EditTaskAction act{id_, t};
    ActionResult result_edit = act.execute(tm_);

    ASSERT_EQ(1, tm_->size());
    EXPECT_TRUE(tm_->Validate(id_));
    EXPECT_EQ(*result_edit.id, id_);

    auto tasks = tm_->getTasks();
    EXPECT_EQ(tasks[0].data().title(), new_title);
    EXPECT_EQ(tasks[0].data().priority(), new_priority);
}

TEST_F(ActionTest, shouldGetTasksToShowWithNoArg)
{
    GetTasksToShowAction act{""};
    ActionResult result = act.execute(tm_);

    ASSERT_EQ(result.type_id, ActionResult::kEntity);
    ASSERT_EQ(1, result.tasks.size());
    EXPECT_EQ(task_, result.tasks[0].data());
}

TEST_F(ActionTest, shouldGetTasksToShowWithLabelArg)
{
    GetTasksToShowAction act{"label"};
    ActionResult result = act.execute(tm_);

    ASSERT_EQ(result.type_id, ActionResult::kEntity);
    ASSERT_EQ(1, result.tasks.size());
    EXPECT_EQ(task_, result.tasks[0].data());
}

TEST_F(ActionTest, shouldGetTasksToShowWithIDArg)
{
    GetTasksToShowAction act{std::to_string(id_.value())};
    ActionResult result = act.execute(tm_);

    ASSERT_EQ(result.type_id, ActionResult::kEntity);
    ASSERT_EQ(1, result.tasks.size());
    EXPECT_EQ(task_, result.tasks[0].data());
}

TEST_F(ActionTest, shouldFailToGetTasksToShowWithInvalidID)
{
    GetTasksToShowAction act{std::to_string(id_.value()) + "0"};
    ActionResult result = act.execute(tm_);

    ASSERT_EQ(result.type_id, ActionResult::kID);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(id_.value(), result.id->value()/10);
}

TEST_F(ActionTest, shouldCompleteTaskWithValidID)
{
    CompleteTaskAction act{id_};
    ActionResult result_complete = act.execute(tm_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_complete.status, ActionResult::Status::SUCCESS);
    EXPECT_TRUE(tm_->getTasks()[0].data().is_complete());
}

TEST_F(ActionTest, shouldNotCompleteTaskWithInvalidID)
{
    Core::TaskID id2;
    id2.set_value(id_.value()+1);
    CompleteTaskAction act{id2};
    ActionResult result_complete = act.execute(tm_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_complete.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_FALSE(tm_->getTasks()[0].data().is_complete());
}

TEST_F(ActionTest, shouldUncompleteTaskWithValidID)
{
    CompleteTaskAction act{id_};
    ActionResult result_complete = act.execute(tm_);

    UncompleteTaskAction act2{id_};
    ActionResult result_uncomplete = act2.execute(tm_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_uncomplete.status, ActionResult::Status::SUCCESS);
    ASSERT_FALSE(tm_->getTasks()[0].data().is_complete());
}

TEST_F(ActionTest, shouldDeleteTaskValidIDNoSubtasks)
{
    DeleteTaskAction act{id_};
    ActionResult result_delete = act.execute(tm_);
    EXPECT_EQ(0, tm_->size());
    EXPECT_EQ(result_delete.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(*result_delete.id, id_);
}

TEST_F(ActionTest, shouldFailToDeleteTaskWithInvalidID)
{
    Core::TaskID id2;
    id2.set_value(id_.value()+1);
    DeleteTaskAction act{id2};
    ActionResult result_delete = act.execute(tm_);
    EXPECT_EQ(1, tm_->size());
    EXPECT_EQ(result_delete.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result_delete.id, id2);
}

TEST_F(ActionTest, shouldDeleteTaskValidIDWithSubtasks)
{
    AddSubtaskAction add{id_, task_};
    add.execute(tm_);
    ASSERT_EQ(2, tm_->size());

    DeleteTaskAction act{id_};
    ActionResult result_delete = act.execute(tm_);
    ASSERT_EQ(0, tm_->size());
    EXPECT_EQ(result_delete.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(*result_delete.id, id_);
}

TEST_F(ActionTest, shouldLabelTask)
{
    std::string label = "custom";
    LabelTaskAction act{id_, label};
    ActionResult result_label = act.execute(tm_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(id_, *result_label.id);
    EXPECT_EQ("label", tm_->getTasks()[0].data().labels()[0]);
    EXPECT_EQ(label, tm_->getTasks()[0].data().labels()[1]);
}

TEST_F(ActionTest, shouldNotLabelTaskWithInvalidID)
{
    Core::TaskID new_id;
    new_id.set_value(id_.value()+1);
    std::string label = "custom";
    LabelTaskAction act{new_id, label};
    ActionResult result_label = act.execute(tm_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_label.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(new_id, result_label.id);
    EXPECT_EQ("label", tm_->getTasks()[0].data().labels()[0]);
}

TEST_F(ActionTest, shouldDoNothing)
{
    DoNothingAction act;
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(std::nullopt, result.id);

}
class MockPersister : public FilePersistence {
public:
    MOCK_METHOD(bool, save, (const std::vector<Core::TaskEntity> &vec), (override));
    MOCK_METHOD(bool, load, (std::vector<Core::TaskEntity> &vec), (override));
};


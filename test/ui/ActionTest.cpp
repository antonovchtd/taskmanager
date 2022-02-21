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
#include "ui/actions/GetAllTasksToShowAction.h"
#include "ui/actions/GetTasksToShowByLabelAction.h"
#include "ui/actions/GetTaskToShowByIDAction.h"
#include "ui/actions/CompleteTaskAction.h"
#include "ui/actions/UncompleteTaskAction.h"
#include "ui/actions/DeleteTaskAction.h"
#include "ui/actions/DoNothingAction.h"
#include "ui/actions/LabelTaskAction.h"
#include "ui/actions/LoadFromFileAction.h"
#include "ui/actions/SaveToFileAction.h"
#include "ui/actions/ClearLabelOfTaskAction.h"
#include "ui/actions/ClearAllLabelsOfTaskAction.h"
#include "ui/actions/GetTaskToShowLabelsAction.h"

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
        id_ = result.model_result->id();
    }

};

TEST_F(ActionTest, shouldAddTask)
{
    ASSERT_EQ(1, tm_->getTasks().size());
    EXPECT_TRUE(ToBool(tm_->IsPresent(id_)));
}

TEST_F(ActionTest, shouldAddSubtask)
{
    auto t = task_;
    t.set_title("sub");
    t.set_priority(Core::Task::Priority::Task_Priority_MEDIUM);
    AddSubtaskAction subact{id_, t};
    ActionResult result_subtask = subact.execute(tm_);
    ASSERT_EQ(2, tm_->getTasks().size());
    ASSERT_TRUE(result_subtask.model_result);
    auto check = tm_->IsPresent(result_subtask.model_result->id());
    ASSERT_TRUE(check.has_id());
    EXPECT_NE(id_, result_subtask.model_result->id());

    auto tasks = tm_->getTasks();
    EXPECT_EQ(id_, tasks[1].parent());
}

TEST_F(ActionTest, shouldExecuteValidateIDValidID)
{
    ValidateIDAction act{id_};
    ActionResult result_validate = act.execute(tm_);
    ASSERT_TRUE(result_validate.tasks);
    EXPECT_EQ(1, result_validate.tasks->size());
}

TEST_F(ActionTest, shouldExecuteValidateIDInvalidID)
{
    Core::TaskID new_id;
    new_id.set_value(id_.value()*10);
    ValidateIDAction act{new_id};
    ActionResult result_validate = act.execute(tm_);
    ASSERT_TRUE(result_validate.model_result);
    ASSERT_TRUE(result_validate.model_result->has_status());
    EXPECT_EQ(result_validate.model_result->status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(ActionTest, shouldExecuteValidateIDNoID)
{
    ValidateIDAction act{std::nullopt};
    ActionResult result_validate = act.execute(tm_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ID);
    EXPECT_FALSE(result_validate);
    EXPECT_FALSE(result_validate.id.has_value());
}

TEST_F(ActionTest, shouldExecuteValidateNoArgActionSomeArg)
{
    ValidateNoArgAction act{"1"};
    ActionResult result = act.execute(tm_);
    ASSERT_TRUE(result.model_result);
    ASSERT_TRUE(result.model_result->has_status());
    EXPECT_EQ(result.model_result->status(), Core::ModelRequestResult_Status_TAKES_NO_ARG);
}

TEST_F(ActionTest, shouldExecuteValidateNoArgActionNoArg)
{
    ValidateNoArgAction act{""};
    ActionResult result = act.execute(tm_);
    ASSERT_TRUE(result.model_result);
    ASSERT_TRUE(result.model_result->has_status());
    EXPECT_EQ(result.model_result->status(), Core::ModelRequestResult_Status_SUCCESS);
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

    ASSERT_EQ(1, tm_->getTasks().size());
    EXPECT_TRUE(ToBool(tm_->IsPresent(id_)));

    ASSERT_TRUE(result_edit.model_result);
    ASSERT_TRUE(result_edit.model_result->has_id());
    EXPECT_EQ(result_edit.model_result->id(), id_);

    auto tasks = tm_->getTasks();
    EXPECT_EQ(tasks[0].data().title(), new_title);
    EXPECT_EQ(tasks[0].data().priority(), new_priority);
}

TEST_F(ActionTest, shouldGetTasksToShowWithNoArg)
{
    GetAllTasksToShowAction act{};
    ActionResult result = act.execute(tm_);

    ASSERT_TRUE(result.tasks);
    ASSERT_EQ(1, result.tasks->size());
    EXPECT_EQ(task_, (*result.tasks)[0].data());
}

TEST_F(ActionTest, shouldGetTasksToShowWithLabelArg)
{
    GetTasksToShowByLabelAction act{"label"};
    ActionResult result = act.execute(tm_);

    ASSERT_TRUE(result.tasks);
    ASSERT_EQ(1, result.tasks->size());
    EXPECT_EQ(task_, (*result.tasks)[0].data());
}

TEST_F(ActionTest, shouldGetTasksToShowWithIDArg)
{
    GetTaskToShowByIDAction act{id_};
    ActionResult result = act.execute(tm_);

    ASSERT_TRUE(result.tasks);
    ASSERT_EQ(1, result.tasks->size());
    EXPECT_EQ(task_, (*result.tasks)[0].data());
}

TEST_F(ActionTest, shouldFailToGetTasksToShowWithInvalidID)
{
    Core::TaskID new_id;
    new_id.set_value(id_.value()*10);
    GetTaskToShowByIDAction act{new_id};
    ActionResult result = act.execute(tm_);

    ASSERT_TRUE(result.model_result);
    ASSERT_TRUE(result.model_result->has_status());
    EXPECT_EQ(result.model_result->status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(ActionTest, shouldCompleteTaskWithValidID)
{
    CompleteTaskAction act{id_};
    ActionResult result_complete = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result_complete.model_result);
    ASSERT_TRUE(result_complete.model_result->has_id());
    EXPECT_EQ(id_, result_complete.model_result->id());
    EXPECT_TRUE(tm_->getTasks()[0].data().is_complete());
}

TEST_F(ActionTest, shouldNotCompleteTaskWithInvalidID)
{
    Core::TaskID id2;
    id2.set_value(id_.value()+1);
    CompleteTaskAction act{id2};
    ActionResult result_complete = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result_complete.model_result);
    ASSERT_TRUE(result_complete.model_result->has_status());
    EXPECT_EQ(result_complete.model_result->status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
    EXPECT_FALSE(tm_->getTasks()[0].data().is_complete());
}

TEST_F(ActionTest, shouldUncompleteTaskWithValidID)
{
    CompleteTaskAction act{id_};
    ActionResult result_complete = act.execute(tm_);

    UncompleteTaskAction act2{id_};
    ActionResult result_uncomplete = act2.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result_uncomplete.model_result);
    EXPECT_TRUE(result_uncomplete.model_result->has_id());
    EXPECT_EQ(result_uncomplete.model_result->id(), id_);
    ASSERT_FALSE(tm_->getTasks()[0].data().is_complete());
}

TEST_F(ActionTest, shouldDeleteTaskValidIDNoSubtasks)
{
    DeleteTaskAction act{id_};
    ActionResult result_delete = act.execute(tm_);
    EXPECT_EQ(0, tm_->getTasks().size());
    ASSERT_TRUE(result_delete.model_result);
    ASSERT_TRUE(result_delete.model_result->has_id());
    EXPECT_EQ(result_delete.model_result->id(), id_);
}

TEST_F(ActionTest, shouldFailToDeleteTaskWithInvalidID)
{
    Core::TaskID id2;
    id2.set_value(id_.value()+1);
    DeleteTaskAction act{id2};
    ActionResult result_delete = act.execute(tm_);
    EXPECT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result_delete.model_result);
    ASSERT_TRUE(result_delete.model_result->has_status());
    EXPECT_EQ(result_delete.model_result->status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(ActionTest, shouldDeleteTaskValidIDWithSubtasks)
{
    AddSubtaskAction add{id_, task_};
    add.execute(tm_);
    ASSERT_EQ(2, tm_->getTasks().size());

    DeleteTaskAction act{id_};
    ActionResult result_delete = act.execute(tm_);
    ASSERT_EQ(0, tm_->getTasks().size());
    ASSERT_TRUE(result_delete.model_result);
    ASSERT_TRUE(result_delete.model_result->has_id());
    EXPECT_EQ(result_delete.model_result->id(), id_);
}

TEST_F(ActionTest, shouldLabelTask)
{
    std::string label = "custom";
    LabelTaskAction act{id_, label};
    ActionResult result_label = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result_label.model_result);
    ASSERT_TRUE(result_label.model_result->has_id());
    EXPECT_EQ(result_label.model_result->id(), id_);
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
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result_label.model_result);
    ASSERT_TRUE(result_label.model_result->has_status());
    EXPECT_EQ(result_label.model_result->status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
    EXPECT_EQ("label", tm_->getTasks()[0].data().labels()[0]);
}

TEST_F(ActionTest, shouldDoNothing)
{
    DoNothingAction act;
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result.model_result);
    ASSERT_TRUE(result.model_result->has_status());
    EXPECT_EQ(result.model_result->status(), Core::ModelRequestResult_Status_SUCCESS);

}

TEST_F(ActionTest, shouldClearAllLabelsOfTask)
{
    LabelTaskAction label_action(id_, "mylabel");
    LabelTaskAction label_action2(id_, "mylabel2");
    label_action.execute(tm_);
    label_action2.execute(tm_);

    ClearAllLabelsOfTaskAction act{id_};
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result.model_result);
    ASSERT_TRUE(result.model_result->has_id());
    EXPECT_EQ(result.model_result->id(), id_);
    auto tasks = tm_->getTaskWithSubtasks(id_);
    EXPECT_TRUE(tasks[0].data().labels().empty());
}

TEST_F(ActionTest, shouldClearOneLabelOfTask)
{
    LabelTaskAction label_action(id_, "mylabel");
    LabelTaskAction label_action2(id_, "mylabel2");
    label_action.execute(tm_);
    label_action2.execute(tm_);

    ClearLabelOfTaskAction act{id_, "mylabel2"};
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    ASSERT_TRUE(result.model_result);
    ASSERT_TRUE(result.model_result->has_id());
    EXPECT_EQ(result.model_result->id(), id_);

    auto tasks = tm_->getTaskWithSubtasks(id_);
    EXPECT_EQ(2, tasks[0].data().labels().size());
    EXPECT_EQ("mylabel", tasks[0].data().labels()[1]);
}

TEST_F(ActionTest, shouldGetTaskToShowItsLabels)
{
    GetTaskToShowLabelsAction act{id_};
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    auto tasks = tm_->getTaskWithSubtasks(id_);
    EXPECT_TRUE(result.entity);
    EXPECT_EQ(tasks[0], *result.entity);
}

TEST_F(ActionTest, shouldFailToGetTaskToShowItsLabelsWithInvalidID)
{
    Core::TaskID new_id;
    new_id.set_value(id_.value()+1);
    GetTaskToShowLabelsAction act{new_id};
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    EXPECT_TRUE(result.model_result);
    EXPECT_EQ(result.model_result->status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(ActionTest, shouldFailToGetTaskToShowItsLabelsWithInvalidArg)
{
    GetTaskToShowLabelsAction act{std::nullopt};
    ActionResult result = act.execute(tm_);
    ASSERT_EQ(1, tm_->getTasks().size());
    EXPECT_TRUE(result.model_result);
    EXPECT_EQ(result.model_result->status(), Core::ModelRequestResult_Status_TAKES_ID);
}
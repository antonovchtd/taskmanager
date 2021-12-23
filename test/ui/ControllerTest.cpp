//
// Created by Anton O. on 12/5/21.
//

#include "ui/Controller.h"
#include "ui/Context.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;
using ::testing::DoAll;

class ControllerTest : public ::testing::Test
{
public:
    std::shared_ptr<TaskManager> tm_;
    Context context_;
    Controller ctr_;

    void SetUp() override {
        tm_ = std::make_shared<TaskManager>();
        ctr_ = Controller{tm_};
        ProtoTask::Task t;
        t.set_title("test");
        t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
        t.set_due_date(time(nullptr));
        t.set_label("label");
        t.set_is_complete(false);
        context_.setTask(t);
    }

};

TEST_F(ControllerTest, shouldAddTask)
{
    ActionResult result = ctr_.AddTask(context_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_TRUE(tm_->Validate(*result.id));
}

TEST_F(ControllerTest, shouldAddSubtask)
{
    ActionResult result = ctr_.AddTask(context_);
    auto t = context_.task();
    t.set_title("sub");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
    context_.setTask(t);
    context_.setID(result.id);
    ActionResult result_subtask = ctr_.AddSubtask(context_);
    ASSERT_EQ(2, tm_->size());
    EXPECT_TRUE(tm_->Validate(*result_subtask.id));
    EXPECT_NE(*result.id, *result_subtask.id);
    EXPECT_EQ(*result.id, (*tm_)[*result_subtask.id].second.parent());
    EXPECT_EQ(*result_subtask.id, (*tm_)[*result.id].second.children()[0]);
}

TEST_F(ControllerTest, shouldValidateIDValidID)
{
    ActionResult result = ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{std::to_string(result.id->value())});
    ActionResult result_validate = ctr_.ValidateID(context_);
    ASSERT_EQ(result_validate.status, ActionResult::Status::SUCCESS);
    EXPECT_TRUE(result_validate);
    EXPECT_EQ(*result.id, *result_validate.id);
    EXPECT_EQ(*context_.id(), *result_validate.id);
}

TEST_F(ControllerTest, shouldValidateIDInvalidID)
{
    ActionResult result = ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{std::to_string(result.id->value()) + "0"});
    ActionResult result_validate = ctr_.ValidateID(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(result_validate.id->value(), result.id->value() * 10);
}

TEST_F(ControllerTest, shouldValidateIDNoID)
{
    ActionResult result = ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{""});
    ActionResult result_validate = ctr_.ValidateID(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ARG);
    EXPECT_EQ(result_validate.id, std::nullopt);
}

TEST_F(ControllerTest, shouldValidateIDBadString)
{
    ctr_.setData(Controller::Data{"bad"});
    ActionResult result = ctr_.ValidateID(context_);
    EXPECT_EQ(result.status, ActionResult::Status::TAKES_ID);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldValidateNoArgSomeID)
{
    ctr_.setData(Controller::Data{"1"});
    ActionResult result = ctr_.ValidateNoArg(context_);
    EXPECT_EQ(result.status, ActionResult::Status::TAKES_NO_ARG);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldValidateNoArgNoID)
{
    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.ValidateNoArg(context_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldValidateLabelOrIdEmptyStr)
{
    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.ValidateLabelOrID(context_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldValidateLabelOrIDValidID)
{
    ActionResult result = ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{std::to_string(result.id->value())});
    ProtoTask::TaskID id_expected;
    id_expected.set_value(1);
    ActionResult result_validate = ctr_.ValidateLabelOrID(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result_validate.id, result.id);
}

TEST_F(ControllerTest, shouldValidateLabelOrIDInvalidID)
{
    ActionResult result = ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{std::to_string(result.id->value()) + "0"});
    ActionResult result_validate = ctr_.ValidateLabelOrID(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(result_validate.id->value(), result.id->value()*10);
}

TEST_F(ControllerTest, shouldFailToValidateAlphaWithID)
{
    ctr_.setData(Controller::Data{"1"});
    ActionResult result_validate = ctr_.ValidateAlpha(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ALPHA);
    EXPECT_EQ(result_validate.id->value(), 1);
}

TEST_F(ControllerTest, shouldFailToValidateAlphaWithText)
{
    ctr_.setData(Controller::Data{"text"});
    ActionResult result_validate = ctr_.ValidateAlpha(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result_validate.id, std::nullopt);
}

TEST_F(ControllerTest, shouldEditTask)
{
    ActionResult result = ctr_.AddTask(context_);
    auto t = context_.task();
    std::string new_title = "edited";
    t.set_title(new_title);
    ProtoTask::Task::Priority new_priority = ProtoTask::Task::Priority::Task_Priority_MEDIUM;
    t.set_priority(new_priority);
    context_.setTask(t);
    context_.setID(result.id);
    ActionResult result_edit = ctr_.EditTask(context_);

    ASSERT_EQ(1, tm_->size());
    EXPECT_TRUE(tm_->Validate(*context_.id()));
    EXPECT_EQ(result_edit.id, context_.id());
    EXPECT_EQ((*tm_)[*result_edit.id].first.title(), new_title);
    EXPECT_EQ((*tm_)[*result_edit.id].first.priority(), new_priority);
}

TEST_F(ControllerTest, shouldShowTasksWithNoArg)
{
    ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.ShowTasks(context_);

    ASSERT_EQ(1, context_.tasks().size());
}

TEST_F(ControllerTest, shouldShowTasksWithLabelArg)
{
    ctr_.AddTask(context_);

    std::string label = "custom";
    ProtoTask::Task t;
    t.set_title("test 2");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label(label);
    t.set_is_complete(false);
    context_.setTask(t);
    ActionResult result_add = ctr_.AddTask(context_);
    auto id = *result_add.id;

    ctr_.setData(Controller::Data{label});
    ActionResult result_show = ctr_.ShowTasks(context_);
    ASSERT_EQ(2, tm_->size());
    ASSERT_EQ(1, context_.tasks().size());
    EXPECT_EQ(context_.tasks()[id].first.title(), "test 2");
}

TEST_F(ControllerTest, shouldShowTasksWithIDArg)
{
    ctr_.AddTask(context_);

    std::string label = "custom";
    ProtoTask::Task t;
    t.set_title("test 2");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label(label);
    t.set_is_complete(false);
    context_.setTask(t);
    ActionResult result_add = ctr_.AddTask(context_);
    auto id = *result_add.id;

    ctr_.setData(Controller::Data{std::to_string(id.value())}); // has to be not empty
    context_.setID(id); // has to be passed as an ID to mimic arg pre-processing
    ActionResult result_show = ctr_.ShowTasks(context_);
    ASSERT_EQ(2, tm_->size());
    ASSERT_EQ(1, context_.tasks().size());
    EXPECT_EQ(context_.tasks()[id].first.title(), "test 2");
}

TEST_F(ControllerTest, shouldCompleteTaskWithValidID)
{
    ActionResult result = ctr_.AddTask(context_);
    context_.setID(result.id);
    ActionResult result_complete = ctr_.CompleteTask(context_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_complete.status, ActionResult::Status::SUCCESS);
    EXPECT_TRUE((*tm_)[*result_complete.id].first.is_complete());
}

TEST_F(ControllerTest, shouldNotCompleteTaskWithInvalidID)
{
    ActionResult result = ctr_.AddTask(context_);
    ProtoTask::TaskID new_id;
    new_id.set_value(result.id->value() + 1);
    context_.setID(new_id);
    ActionResult result_complete = ctr_.CompleteTask(context_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_complete.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_FALSE((*tm_)[*result.id].first.is_complete());
}

TEST_F(ControllerTest, shouldDeleteTaskValidIDNoSubtasks)
{
    ActionResult result = ctr_.AddTask(context_);
    context_.setID(result.id);
    ActionResult result_delete = ctr_.DeleteTask(context_);
    EXPECT_EQ(0, tm_->size());
    EXPECT_EQ(result_delete.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result_delete.id, result.id);
}

TEST_F(ControllerTest, shouldGetTasksForConfirmDelete)
{
    ActionResult result = ctr_.AddTask(context_);
    context_.setID(result.id);
    ctr_.AddSubtask(context_);

    ActionResult result_confirm = ctr_.ConfirmDeleteTask(context_);
    EXPECT_EQ(2, tm_->size());
    EXPECT_EQ(2, context_.tasks().size());
    EXPECT_EQ(result_confirm.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result_confirm.id, result.id);
}

TEST_F(ControllerTest, shouldGetTasksForConfirmDeleteWithWrongID)
{
    ActionResult result = ctr_.AddTask(context_);
    ProtoTask::TaskID new_id;
    new_id.set_value(result.id->value() + 1);
    context_.setID(new_id);

    ActionResult result_confirm = ctr_.ConfirmDeleteTask(context_);
    EXPECT_EQ(1, tm_->size());
    EXPECT_EQ(0, context_.tasks().size());
    EXPECT_EQ(result_confirm.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result_confirm.id, new_id);
}

TEST_F(ControllerTest, shouldDeleteTaskInvalidIDNoSubtasks)
{
    ActionResult result = ctr_.AddTask(context_);
    ProtoTask::TaskID new_id;
    new_id.set_value(result.id->value() + 1);
    context_.setID(new_id);
    ActionResult result_delete = ctr_.DeleteTask(context_);
    EXPECT_EQ(1, tm_->size());
    EXPECT_EQ(result_delete.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(result_delete.id, new_id);
}

TEST_F(ControllerTest, shouldDeleteTaskValidIDWithSubtasks)
{
    ActionResult result = ctr_.AddTask(context_);
    context_.setID(result.id);
    ActionResult result_sub = ctr_.AddSubtask(context_);
    ActionResult result_delete = ctr_.DeleteTask(context_);
    EXPECT_EQ(0, tm_->size());
    EXPECT_EQ(result_delete.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result_delete.id, result.id);
}

TEST_F(ControllerTest, shouldLabelTask)
{
    ActionResult result = ctr_.AddTask(context_);
    context_.setID(result.id);
    std::string label = "custom";
    ctr_.setData(Controller::Data{label});
    ActionResult result_label = ctr_.LabelTask(context_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result.id, result_label.id);
    EXPECT_EQ(label, (*tm_)[*result_label.id].first.label());
}

TEST_F(ControllerTest, shouldNotLabelTaskWithInvalidID)
{
    ActionResult result = ctr_.AddTask(context_);
    ProtoTask::TaskID new_id;
    new_id.set_value(result.id->value()+1);
    context_.setID(new_id);
    std::string label = "custom";
    ctr_.setData(Controller::Data{label});
    ActionResult result_label = ctr_.LabelTask(context_);
    ASSERT_EQ(1, tm_->size());
    EXPECT_EQ(result_label.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(new_id, result_label.id);
    EXPECT_EQ("label", (*tm_)[*result.id].first.label());
}

class MockPersister : public Persister {
public:
    MOCK_METHOD(bool, save, (const std::string &filename, const std::shared_ptr<TaskManager> &model), (override));
    MOCK_METHOD(bool, load, (const std::string &filename, const std::shared_ptr<TaskManager> &model), (override));
};

TEST_F(ControllerTest, shouldReturnSuccesOnCorrectSaveWithCustomFilename)
{
    std::shared_ptr<MockPersister> mp = std::make_shared<MockPersister>();
    ctr_ = Controller{tm_, mp};
    std::string expected_filename = "custom.txt";

    EXPECT_CALL(*mp, save(expected_filename, tm_))
            .Times(1)
            .WillOnce(Return(true));

    ctr_.setData(Controller::Data{expected_filename});
    ActionResult result = ctr_.SaveTasks(context_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldReturnSuccesOnCorrectSaveWithDefaultFilename)
{
    std::shared_ptr<MockPersister> mp = std::make_shared<MockPersister>();
    ctr_ = Controller{tm_, mp};

    EXPECT_CALL(*mp, save(mp->defaultFilename(), tm_))
            .Times(1)
            .WillOnce(Return(true));

    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.SaveTasks(context_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldReturnFailOnIncorrectSaveWithDefaultFilename)
{
    std::shared_ptr<MockPersister> mp = std::make_shared<MockPersister>();
    ctr_ = Controller{tm_, mp};

    EXPECT_CALL(*mp, save(mp->defaultFilename(), tm_))
            .Times(1)
            .WillOnce(Return(false));

    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.SaveTasks(context_);
    EXPECT_EQ(result.status, ActionResult::Status::FAILED_TO_OPEN_FILE);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldReturnSuccesOnCorrectLoadWithCustomFilename)
{
    std::shared_ptr<MockPersister> mp = std::make_shared<MockPersister>();
    ctr_ = Controller{tm_, mp};
    std::string expected_filename = "custom.txt";

    EXPECT_CALL(*mp, load(expected_filename, tm_))
            .Times(1)
            .WillOnce(Return(true));

    ctr_.setData(Controller::Data{expected_filename});
    ActionResult result = ctr_.LoadTasks(context_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldReturnSuccesOnCorrectLoadWithDefaultFilename)
{
    std::shared_ptr<MockPersister> mp = std::make_shared<MockPersister>();
    ctr_ = Controller{tm_, mp};

    EXPECT_CALL(*mp, load(mp->defaultFilename(), tm_))
            .Times(1)
            .WillOnce(Return(true));

    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.LoadTasks(context_);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(result.id, std::nullopt);
}

TEST_F(ControllerTest, shouldReturnFailOnIncorrectLoadWithDefaultFilename)
{
    std::shared_ptr<MockPersister> mp = std::make_shared<MockPersister>();
    ctr_ = Controller{tm_, mp};

    EXPECT_CALL(*mp, load(mp->defaultFilename(), tm_))
            .Times(1)
            .WillOnce(Return(false));

    ctr_.setData(Controller::Data{""});
    ActionResult result = ctr_.LoadTasks(context_);
    EXPECT_EQ(result.status, ActionResult::Status::FILE_NOT_FOUND);
    EXPECT_EQ(result.id, std::nullopt);
}
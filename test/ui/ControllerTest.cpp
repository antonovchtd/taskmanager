//
// Created by Anton O. on 12/5/21.
//

#include "../../src/ui/Controller.h"
#include "../../src/ui/Context.h"

#include <gtest/gtest.h>

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
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ID);
    EXPECT_EQ(result_validate.id, std::nullopt);
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
    ActionResult result = ctr_.AddTask(context_);
    ctr_.setData(Controller::Data{"bad"});
    ActionResult result_validate = ctr_.ValidateID(context_);
    EXPECT_EQ(result_validate.status, ActionResult::Status::TAKES_ID);
    EXPECT_EQ(result_validate.id, std::nullopt);
}
//
//TEST_F(ControllerTest, makeValidateNoArgActionSomeID)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ValidateNoArgAction vnaa(ptr, Action::Data{"1"});
//    vnaa.make(c);
//    EXPECT_EQ(std::nullopt, c.id());
//}
//
//TEST_F(ControllerTest, makeValidateNoArgActionNoID)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ValidateNoArgAction vnaa(ptr, Action::Data{""});
//    vnaa.make(c);
//    EXPECT_TRUE(c.id()->has_is_invalid() && !c.id()->is_invalid());
//}
//
//TEST_F(ControllerTest, makeValidateLabelArgActionEmptyStr)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ValidateLabelOrIDArgAction vlaa(ptr, Action::Data{""});
//    vlaa.make(c);
//    EXPECT_TRUE(c.id()->has_is_invalid() && !c.id()->is_invalid());
//}
//
//TEST_F(ControllerTest, makeValidateLabelArgActionValidID)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("label");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//    ValidateLabelOrIDArgAction vlaa(ptr, Action::Data{std::to_string(id.num())});
//    vlaa.make(c);
//    EXPECT_EQ(id, *c.id());
//}
//
//TEST_F(ControllerTest, makeValidateLabelArgActionInvalidID)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("label");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//    ValidateLabelOrIDArgAction vlaa(ptr, Action::Data{std::to_string(id.num()) + "0"});
//    vlaa.make(c);
//    EXPECT_TRUE(c.id()->has_is_invalid() && c.id()->is_invalid());
//}
//
//TEST_F(ControllerTest, makeEditTaskAction)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("label");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//    t.set_title("edited");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
//    c.setTask(t);
//    EditTaskAction eta(ptr);
//    c.setID(id);
//    eta.make(c);
//    ASSERT_EQ(1, ptr->size());
//    EXPECT_TRUE(ptr->Validate(*c.id()));
//    EXPECT_EQ(id, *c.id());
//    EXPECT_EQ((*ptr)[id].first.title(), "edited");
//    EXPECT_EQ((*ptr)[id].first.priority(), ProtoTask::Task::Priority::Task_Priority_MEDIUM);
//}
//
//TEST_F(ControllerTest, makeShowActionNoArg)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("label");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//    ShowAction sa(ptr, Action::Data{""});
//    sa.make(c);
//    ASSERT_EQ(1, c.tasks().size());
//    EXPECT_EQ(c.tasks().at(id).first.title(), "test");
//}
//
//TEST_F(ControllerTest, makeShowActionLabelArg)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    std::string label = "l";
//
//    ProtoTask::Task t;
//    t.set_title("test");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("");
//    t.set_is_complete(false);
//    ptr->Add(t);
//
//    t.set_title("test 2");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
//    t.set_label(label);
//    ProtoTask::TaskID id = ptr->Add(t);
//
//    ShowAction sa(ptr, Action::Data{label});
//    sa.make(c);
//    ASSERT_EQ(1, c.tasks().size());
//    EXPECT_EQ(c.tasks().at(id).first.title(), "test 2");
//}
//
//TEST_F(ControllerTest, makeShowActionIDArg)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test 1");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//
//    t.set_title("test 2");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
//
//    ptr->Add(t);
//    ShowAction sa(ptr, Action::Data{std::to_string(id.num())});
//    c.setID(id);
//    sa.make(c);
//    ASSERT_EQ(1, c.tasks().size());
//    EXPECT_EQ(c.tasks().at(id).first.title(), "test 1");
//}
//
//TEST_F(ControllerTest, makeCompleteTaskAction)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//
//    CompleteTaskAction ca(ptr);
//    c.setID(id);
//    ca.make(c);
//    ASSERT_EQ(1, ptr->size());
//    EXPECT_TRUE((*ptr)[id].first.is_complete());
//}
//
//TEST_F(ControllerTest, makeDeleteTaskAction)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test 1");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//
//    DeleteAction da(ptr);
//    c.setID(id);
//    da.make(c);
//    EXPECT_EQ(0, ptr->size());
//}
//
//TEST_F(ControllerTest, makeLabelAction)
//{
//    auto ptr = std::make_shared<TaskManager>(TaskManager{});
//    Context c;
//    ProtoTask::Task t;
//    t.set_title("test 1");
//    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
//    t.set_due_date(time(nullptr));
//    t.set_label("");
//    t.set_is_complete(false);
//    ProtoTask::TaskID id = ptr->Add(t);
//
//    std::string label = "l";
//    LabelAction la(ptr, Action::Data{label});
//    c.setID(id);
//    la.make(c);
//    ASSERT_EQ(1, ptr->size());
//    EXPECT_EQ(label, (*ptr)[id].first.label());
//}
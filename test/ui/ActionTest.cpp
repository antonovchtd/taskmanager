//
// Created by Anton O. on 12/5/21.
//

#include "../../src/ui/Controller.h"
#include "../../src/ui/Context.h"

#include <gtest/gtest.h>

class ActionTest : public ::testing::Test
{

};

TEST_F(ActionTest, makeAddTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    c.setTask(t);
    AddTaskAction ata(ptr);
    ata.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_TRUE(ptr->Validate(*c.id()));
}

TEST_F(ActionTest, makeAddSubtaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    t.set_title("sub");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);

    c.setTask(t);
    AddSubtaskAction asa(ptr);
    c.setID(id);
    asa.make(c);
    ASSERT_EQ(2, ptr->size());
    EXPECT_TRUE(ptr->Validate(*c.id()));
    EXPECT_NE(id, *c.id());
}

TEST_F(ActionTest, makeValidateIDActionValidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    GetIDAction via(ptr, Action::Data{std::to_string(id.num())});
    via.make(c);
    EXPECT_EQ(id, *c.id());
}

TEST_F(ActionTest, makeValidateIDActionInvalidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    GetIDAction via(ptr, Action::Data{std::to_string(id.num()) + "0"});
    via.make(c);
    EXPECT_TRUE(c.id()->has_is_invalid() && c.id()->is_invalid());
}

TEST_F(ActionTest, makeValidateIDActionNoID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    GetIDAction via(ptr, Action::Data{""});
    via.make(c);
    EXPECT_TRUE(c.id()->has_is_invalid() && c.id()->is_invalid());
}

TEST_F(ActionTest, makeValidateIDActionString)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    GetIDAction via(ptr, Action::Data{"bad"});
    via.make(c);
    EXPECT_TRUE(c.id()->has_is_invalid() && c.id()->is_invalid());
}

TEST_F(ActionTest, makeValidateNoArgActionSomeID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ValidateNoArgAction vnaa(ptr, Action::Data{"1"});
    vnaa.make(c);
    EXPECT_EQ(std::nullopt, c.id());
}

TEST_F(ActionTest, makeValidateNoArgActionNoID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ValidateNoArgAction vnaa(ptr, Action::Data{""});
    vnaa.make(c);
    EXPECT_TRUE(c.id()->has_is_invalid() && !c.id()->is_invalid());
}

TEST_F(ActionTest, makeValidateLabelArgActionEmptyStr)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ValidateLabelOrIDArgAction vlaa(ptr, Action::Data{""});
    vlaa.make(c);
    EXPECT_TRUE(c.id()->has_is_invalid() && !c.id()->is_invalid());
}

TEST_F(ActionTest, makeValidateLabelArgActionValidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    ValidateLabelOrIDArgAction vlaa(ptr, Action::Data{std::to_string(id.num())});
    vlaa.make(c);
    EXPECT_EQ(id, *c.id());
}

TEST_F(ActionTest, makeValidateLabelArgActionInvalidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    ValidateLabelOrIDArgAction vlaa(ptr, Action::Data{std::to_string(id.num()) + "0"});
    vlaa.make(c);
    EXPECT_TRUE(c.id()->has_is_invalid() && c.id()->is_invalid());
}

TEST_F(ActionTest, makeEditTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    t.set_title("edited");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
    c.setTask(t);
    EditTaskAction eta(ptr);
    c.setID(id);
    eta.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_TRUE(ptr->Validate(*c.id()));
    EXPECT_EQ(id, *c.id());
    EXPECT_EQ((*ptr)[id].first.title(), "edited");
    EXPECT_EQ((*ptr)[id].first.priority(), ProtoTask::Task::Priority::Task_Priority_MEDIUM);
}

TEST_F(ActionTest, makeShowActionNoArg)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);
    ShowAction sa(ptr, Action::Data{""});
    sa.make(c);
    ASSERT_EQ(1, c.tasks().size());
    EXPECT_EQ(c.tasks().at(id).first.title(), "test");
}

TEST_F(ActionTest, makeShowActionLabelArg)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    std::string label = "l";

    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ptr->Add(t);

    t.set_title("test 2");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
    t.set_label(label);
    ProtoTask::TaskID id = ptr->Add(t);

    ShowAction sa(ptr, Action::Data{label});
    sa.make(c);
    ASSERT_EQ(1, c.tasks().size());
    EXPECT_EQ(c.tasks().at(id).first.title(), "test 2");
}

TEST_F(ActionTest, makeShowActionIDArg)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test 1");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);

    t.set_title("test 2");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);

    ptr->Add(t);
    ShowAction sa(ptr, Action::Data{std::to_string(id.num())});
    c.setID(id);
    sa.make(c);
    ASSERT_EQ(1, c.tasks().size());
    EXPECT_EQ(c.tasks().at(id).first.title(), "test 1");
}

TEST_F(ActionTest, makeCompleteTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);

    CompleteTaskAction ca(ptr);
    c.setID(id);
    ca.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_TRUE((*ptr)[id].first.is_complete());
}

TEST_F(ActionTest, makeDeleteTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test 1");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);

    DeleteAction da(ptr);
    c.setID(id);
    da.make(c);
    EXPECT_EQ(0, ptr->size());
}

TEST_F(ActionTest, makeLabelAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ProtoTask::Task t;
    t.set_title("test 1");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_HIGH);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = ptr->Add(t);

    std::string label = "l";
    LabelAction la(ptr, Action::Data{label});
    c.setID(id);
    la.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_EQ(label, (*ptr)[id].first.label());
}
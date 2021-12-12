//
// Created by Anton O. on 12/5/21.
//

#include "../../src/ui/Action.h"
#include "../../src/ui/Context.h"

#include <gtest/gtest.h>

class ActionTest : public ::testing::Test
{

};

TEST_F(ActionTest, makeAddTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    c.setData(Task::Data{"test", Task::Priority::HIGH, time(nullptr), "label", false});
    AddTaskAction ata(ptr);
    ata.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_TRUE(ptr->Validate(*c.id()));
}

TEST_F(ActionTest, makeAddSubtaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "l", false));
    c.setData(Task::Data{"sub", Task::Priority::MEDIUM, time(nullptr), "l", false});
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
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    ValidateIDAction via(ptr, Action::Data{id.to_string()});
    via.make(c);
    EXPECT_EQ(id, *c.id());
}

TEST_F(ActionTest, makeValidateIDActionInvalidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    ValidateIDAction via(ptr, Action::Data{id.to_string() + "0"});
    via.make(c);
    EXPECT_EQ(TaskID::invalidID(), *c.id());
}

TEST_F(ActionTest, makeValidateIDActionNoID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ValidateIDAction via(ptr, Action::Data{""});
    via.make(c);
    EXPECT_EQ(TaskID::invalidID(), *c.id());
}

TEST_F(ActionTest, makeValidateIDActionString)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ValidateIDAction via(ptr, Action::Data{"bad"});
    via.make(c);
    EXPECT_EQ(TaskID::invalidID(), *c.id());
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
    EXPECT_EQ(TaskID::nullid(), *c.id());
}

TEST_F(ActionTest, makeValidateLabelArgActionEmptyStr)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    ValidateLabelArgAction vlaa(ptr, Action::Data{""});
    vlaa.make(c);
    EXPECT_EQ(TaskID::nullid(), *c.id());
}

TEST_F(ActionTest, makeValidateLabelArgActionValidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    ValidateLabelArgAction vlaa(ptr, Action::Data{id.to_string()});
    vlaa.make(c);
    EXPECT_EQ(id, *c.id());
}

TEST_F(ActionTest, makeValidateLabelArgActionInvalidID)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    ValidateLabelArgAction vlaa(ptr, Action::Data{id.to_string() + "0"});
    vlaa.make(c);
    EXPECT_EQ(TaskID::invalidID(), *c.id());
}

TEST_F(ActionTest, makeEditTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    c.setData(Task::Data{"edited", Task::Priority::MEDIUM, time(nullptr), "", false});
    EditTaskAction eta(ptr);
    c.setID(id);
    eta.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_TRUE(ptr->Validate(*c.id()));
    EXPECT_EQ(id, *c.id());
    EXPECT_EQ((*ptr)[id].first.title(), "edited");
    EXPECT_EQ((*ptr)[id].first.priority(), Task::Priority::MEDIUM);
}

TEST_F(ActionTest, makeShowActionNoArg)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
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
    ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    TaskID id = ptr->Add(Task::Create("test 2", Task::Priority::MEDIUM, time(nullptr), label, false));
    ShowAction sa(ptr, Action::Data{label});
    sa.make(c);
    ASSERT_EQ(1, c.tasks().size());
    EXPECT_EQ(c.tasks().at(id).first.title(), "test 2");
}

TEST_F(ActionTest, makeShowActionIDArg)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test 1", Task::Priority::HIGH, time(nullptr), "", false));
    ptr->Add(Task::Create("test 2", Task::Priority::MEDIUM, time(nullptr), "", false));
    ShowAction sa(ptr, Action::Data{id.to_string()});
    c.setID(id);
    sa.make(c);
    ASSERT_EQ(1, c.tasks().size());
    EXPECT_EQ(c.tasks().at(id).first.title(), "test 1");
}

TEST_F(ActionTest, makeCompleteTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    CompleteTaskAction ca(ptr);
    c.setID(id);
    ca.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_TRUE((*ptr)[id].first.isComplete());
}

TEST_F(ActionTest, makeDeleteTaskAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    DeleteAction da(ptr);
    c.setID(id);
    da.make(c);
    EXPECT_EQ(0, ptr->size());
}

TEST_F(ActionTest, makeLabelAction)
{
    auto ptr = std::make_shared<TaskManager>(TaskManager{});
    Context c;
    std::string label = "l";
    TaskID id = ptr->Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), "", false));
    LabelAction la(ptr, Action::Data{label});
    c.setID(id);
    la.make(c);
    ASSERT_EQ(1, ptr->size());
    EXPECT_EQ(label, (*ptr)[id].first.label());
}
//
// Created by Anton O. on 12/5/21.
//

#include "../../src/controller/Action.h"

#include <gtest/gtest.h>

class ActionTest : public ::testing::Test
{

};

TEST_F(ActionTest, makeAddTaskAction)
{
    TaskManager tm;
    Context c;
    c.setData(Task::Data{"test", Task::Priority::HIGH, time(nullptr), false});
    AddTaskAction ata;
    ata.make(tm, c);
    ASSERT_EQ(1,tm.size());
    EXPECT_TRUE(tm.Validate(c.id().value()));
}

TEST_F(ActionTest, makeAddSubtaskAction)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    c.setData(Task::Data{"sub", Task::Priority::MEDIUM, time(nullptr), false});
    AddSubtaskAction asa;
    c.setID(id);
    asa.make(tm, c);
    ASSERT_EQ(2,tm.size());
    EXPECT_TRUE(tm.Validate(c.id().value()));
    EXPECT_NE(id, c.id().value());
}

TEST_F(ActionTest, makeValidateIDActionValidID)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    c.setArg(id.to_string());
    ValidateIDAction via;
    via.make(tm, c);
    EXPECT_EQ(id, c.id().value());
}

TEST_F(ActionTest, makeValidateIDActionInvalidID)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    c.setArg(id.to_string() + "0");
    ValidateIDAction via;
    via.make(tm, c);
    EXPECT_EQ(c.id(), TaskID::invalidID());
}

TEST_F(ActionTest, makeValidateIDActionNoID)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    c.setArg("");
    ValidateIDAction via;
    via.make(tm, c);
    EXPECT_EQ(c.id(), TaskID::invalidID());
}

TEST_F(ActionTest, makeValidateNoArgActionSomeID)
{
    TaskManager tm;
    Context c;
    Factory f;
    c.setOldStep(f.getHomeStep());
    c.setStep(f.getHelpStep());
    c.setArg("1");
    ValidateNoArgAction via;
    via.make(tm, c);
    EXPECT_FALSE(c.id());
    EXPECT_EQ(c.getStep(), c.getOldStep());
}

TEST_F(ActionTest, makeValidateNoArgActionNoID)
{
    TaskManager tm;
    Context c;
    c.setArg("");
    ValidateNoArgAction via;
    via.make(tm, c);
    EXPECT_EQ(c.id(), TaskID::nullid());
}

TEST_F(ActionTest, makeValidateLabelArgAction)
{
    TaskManager tm;
    Context c;
    c.setArg("");
    ValidateLabelArgAction vlaa;
    vlaa.make(tm, c);
    EXPECT_EQ(c.id(), TaskID::nullid());
}

TEST_F(ActionTest, makeEditTaskAction)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    c.setData(Task::Data{"edited", Task::Priority::MEDIUM, time(nullptr), false});
    EditTaskAction eta;
    c.setID(id);
    eta.make(tm, c);
    ASSERT_EQ(1,tm.size());
    EXPECT_TRUE(tm.Validate(c.id().value()));
    EXPECT_EQ(id, c.id().value());
    EXPECT_EQ(tm[id].first.title(), "edited");
}

TEST_F(ActionTest, makeShowActionNoArg)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    ShowAction sa;
    c.setArg("");
    sa.make(tm, c);
    ASSERT_EQ(1,c.getTasks().size());
    EXPECT_EQ(c.getTasks()[id].first.title(), "test");
}

TEST_F(ActionTest, makeShowActionLabelArg)
{
    TaskManager tm;
    Context c;
    std::string label = "l";
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    tm.Add(Task::Create("test 2", Task::Priority::MEDIUM, time(nullptr), false));
    tm.SetLabel(id, label);
    ShowAction sa;
    c.setArg(label);
    sa.make(tm, c);
    ASSERT_EQ(1,c.getTasks().size());
    EXPECT_EQ(c.getTasks()[id].first.title(), "test");
}

TEST_F(ActionTest, makeCompleteTaskAction)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    CompleteTaskAction ca;
    c.setID(id);
    ca.make(tm, c);
    ASSERT_EQ(1,tm.size());
    EXPECT_TRUE(tm[id].first.isComplete());
}

TEST_F(ActionTest, makeDeleteTaskAction)
{
    TaskManager tm;
    Context c;
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    DeleteAction da;
    c.setID(id);
    da.make(tm, c);
    EXPECT_EQ(0, tm.size());
}

TEST_F(ActionTest, makeLabelAction)
{
    TaskManager tm;
    Context c;
    std::string label = "l";
    TaskID id = tm.Add(Task::Create("test", Task::Priority::HIGH, time(nullptr), false));
    LabelAction la;
    c.setID(id);
    c.setLabel(label);
    la.make(tm, c);
    ASSERT_EQ(1, tm.size());
    EXPECT_EQ(label, tm[id].second.label());
}
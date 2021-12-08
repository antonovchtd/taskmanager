//
// Created by Anton O. on 12/3/21.
//

#include "../../src/ui/Context.h"
#include "../../src/ui/Factory.h"
#include "../../src/model/TaskManager.h"

#include <gtest/gtest.h>

class ContextTest : public ::testing::Test
{

};

TEST_F(ContextTest, shouldSetAndGetStep)
{
    Context c;
    auto f = Factory::create();
    c.setStep(f->getStep(Factory::State::HOME));
    EXPECT_EQ(c.getStep(), f->getStep(Factory::State::HOME));
}

TEST_F(ContextTest, shouldSetAndGetOldStep)
{
    Context c;
    auto f = Factory::create();
    c.setOldStep(f->getStep(Factory::State::HOME));
    EXPECT_EQ(c.getOldStep(), f->getStep(Factory::State::HOME));
    EXPECT_NE(c.getOldStep(), f->getStep(Factory::State::HELP));
}

TEST_F(ContextTest, shouldChangeStep)
{
    Context c;
    auto f = Factory::create();
    c.setStep(f->getStep(Factory::State::HOME));
    c.setStep(f->getStep(Factory::State::HELP));
    EXPECT_EQ(c.getOldStep(), f->getStep(Factory::State::HOME));
    EXPECT_EQ(c.getStep(), f->getStep(Factory::State::HELP));
}

TEST_F(ContextTest, shouldRevertStep)
{
    Context c;
    auto f = Factory::create();
    c.setStep(f->getStep(Factory::State::HOME));
    c.setStep(f->getStep(Factory::State::HELP));
    c.revertStep();
    EXPECT_EQ(c.getOldStep(), c.getStep());
}

TEST_F(ContextTest, shouldSetID)
{
    Context c;
    TaskID id = TaskID(42);
    c.setID(id);
    EXPECT_EQ(id, c.id());
}

TEST_F(ContextTest, shouldSetTasks)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("test",
                                Task::Priority::NONE,
                                time(nullptr),
                                false));
    Context c;
    c.setTasks(tm.getTasks());
    auto tasks = c.getTasks();
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ(tm[id].first, tasks[id].first);
}

TEST_F(ContextTest, shouldAddLabel) {
    Context c;
    std::string label = "test label";
    c.setLabel(label);
    EXPECT_EQ(label, c.label());
}

TEST_F(ContextTest, shouldAddArg){
    Context c;
    std::string arg = "test arg";
    c.setArg(arg);
    EXPECT_EQ(arg, c.arg());
}

TEST_F(ContextTest, shouldSetData){
    Task::Data data = {"test", Task::Priority::NONE, time(nullptr), false};
    Context c;
    c.setData(data);
    EXPECT_EQ(data.title, c.data().title);
    EXPECT_EQ(data.priority, c.data().priority);
    EXPECT_EQ(data.due_date, c.data().due_date);
    EXPECT_EQ(data.is_complete, c.data().is_complete);
}

TEST_F(ContextTest, shouldResetTaskData){
    Task::Data data = {"test", Task::Priority::NONE, time(nullptr), false};
    Context c;
    c.setData(data);
    c.setID(TaskID(42));
    c.resetTaskData();
    EXPECT_EQ("", c.data().title);
    EXPECT_EQ(TaskID::invalidID(), c.id());
}

TEST_F(ContextTest, shouldSetTitle){
    std::string title = "test title";
    Context c;
    c.setTitle(title);
    EXPECT_EQ(title, c.data().title);
}

TEST_F(ContextTest, shouldSetDueDate){
    time_t t = time(nullptr);
    Context c;
    c.setDueDate(t);
    EXPECT_EQ(t, c.data().due_date);
}

TEST_F(ContextTest, shouldSetPriority){
    Task::Priority p = Task::Priority::NONE;
    Context c;
    c.setPriority(p);
    EXPECT_EQ(p, c.data().priority);
}
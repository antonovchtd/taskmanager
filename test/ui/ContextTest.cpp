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
    c.setStep(f->lazyInitStep(Factory::State::HOME));
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::HOME));
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
                                "label",
                                false));
    Context c;
    c.setTasks(tm.getTasks());
    auto tasks = c.tasks();
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ(tm[id].first, tasks.at(id).first);
}

TEST_F(ContextTest, shouldSetData){
    Task::Data data = {"test", Task::Priority::NONE, time(nullptr), "label", false};
    Context c;
    c.setData(data);
    EXPECT_EQ(data.title, c.data().title);
    EXPECT_EQ(data.priority, c.data().priority);
    EXPECT_EQ(data.due_date, c.data().due_date);
    EXPECT_EQ(data.label, c.data().label);
    EXPECT_EQ(data.is_complete, c.data().is_complete);
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
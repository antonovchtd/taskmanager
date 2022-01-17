//
// Created by Anton O. on 12/3/21.
//

#include "ui/Context.h"
#include "utilities/TaskEntityUtils.h"

#include <gtest/gtest.h>

class ContextTest : public ::testing::Test
{

};

TEST_F(ContextTest, shouldSetID)
{
    Context c;
    Core::TaskID id;
    id.set_value(42);
    c.setID(id);
    EXPECT_EQ(id, *c.id());
}

TEST_F(ContextTest, shouldSetTasks)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("test");
    t.set_priority(Core::Task_Priority_NONE);
    t.set_label("label");
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    Context c;
    auto tasks = tm.getTasks();
    c.setTasks(tasks);
    auto tasks_from_context = c.tasks();
    ASSERT_EQ(tasks_from_context.size(), tasks.size());
    ASSERT_EQ(tasks_from_context.size(), 1);
    EXPECT_EQ(tasks_from_context[0], tasks[0]);
}

TEST_F(ContextTest, shouldSetData){
    Core::Task t;
    t.set_title("test");
    t.set_priority(Core::Task_Priority_NONE);
    t.set_label("label");
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Context c;
    c.setTask(t);
    EXPECT_EQ(t.title(), c.task().title());
    EXPECT_EQ(t.priority(), c.task().priority());
    EXPECT_EQ(t.due_date(), c.task().due_date());
    EXPECT_EQ(t.label(), c.task().label());
    EXPECT_EQ(t.is_complete(), c.task().is_complete());
}

TEST_F(ContextTest, shouldSetTitle){
    std::string title = "test title";
    Context c;
    c.setTitle(title);
    EXPECT_EQ(title, c.task().title());
}

TEST_F(ContextTest, shouldSetDueDate){
    time_t t = time(nullptr);
    Context c;
    c.setDueDate(t);
    EXPECT_EQ(t, c.task().due_date());
}

TEST_F(ContextTest, shouldSetPriority){
    Core::Task::Priority p = Core::Task_Priority_MEDIUM;
    Context c;
    c.setPriority(p);
    EXPECT_EQ(p, c.task().priority());
}
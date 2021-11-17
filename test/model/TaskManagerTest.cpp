//
// Created by Anton Ovcharenko on 17.11.2021.
//

//
// Created by Anton O. on 11/12/21.
//

#include "../../src/model/Task.h"
#include "../../src/model/TaskManager.h"

#include <gtest/gtest.h>

class TaskManagerTest : public ::testing::Test
{

};

TEST_F(TaskManagerTest, shouldAddTask)
{
    TaskManager tm;
    auto id = tm.Add(Task::Create("TestTitle"));
    EXPECT_TRUE(tm.Validate(TaskID(id)));
}

TEST_F(TaskManagerTest, shouldEdit)
{
    Task t = Task::Create("My Test Task", Task::Priority::MEDIUM, "16/11/2021 19:00");
    TaskManager tm;
    TaskID id = tm.Add(Task{});
    tm.Edit(id, t);
    auto tasks = tm.getTasks();
    EXPECT_EQ(t, tasks[id]);
}

TEST_F(TaskManagerTest, shouldDeleteTask)
{
    TaskManager tm;
    auto id = tm.Add(Task::Create("TestTitle"));
    tm.Delete(id);
    EXPECT_FALSE(tm.Validate(TaskID(id)));
}

TEST_F(TaskManagerTest, shouldCompleteTask)
{
    TaskManager tm;
    auto id = tm.Add(Task::Create("TestTitle"));
    tm.Complete(id);
    auto tasks = tm.getTasks();
    EXPECT_TRUE(tasks.find(id)->second.isComplete());
}

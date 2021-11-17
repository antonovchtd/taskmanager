//
// Created by Anton Ovcharenko on 17.11.2021.
//

//
// Created by Anton O. on 11/12/21.
//

#include "../../src/model/TaskManager.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;

class TaskManagerTest : public ::testing::Test
{

};

class MockIDGenerator : public IDGenerator
{
public:
    MockIDGenerator(unsigned int last) {last_ = last;};
    MockIDGenerator() {last_ = 0;};
    MOCK_METHOD(TaskID, genID, (), (override));
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

TEST_F(TaskManagerTest, shouldAddMultipleTasks)
{
    std::shared_ptr<MockIDGenerator> generator = std::shared_ptr<MockIDGenerator>(new MockIDGenerator);
    EXPECT_CALL(*generator, genID())
        .Times(3)
        .WillOnce(Return(TaskID(1)))
        .WillOnce(Return(TaskID(2)))
        .WillOnce(Return(TaskID(3)));

    TaskManager tm(generator);
    Task t1 = Task::Create("My Test Task 1", Task::Priority::MEDIUM, "30/11/2021 19:00");
    Task t2 = Task::Create("My Test Task 2", Task::Priority::MEDIUM, "in 02:00:00");
    TaskID id1 = tm.Add(t1);
    TaskID id2 = tm.Add(t2);
    TaskID id3 = tm.Add(t2);
    EXPECT_EQ(3,tm.size());
}
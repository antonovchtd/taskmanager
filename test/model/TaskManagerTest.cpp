//
// Created by Anton Ovcharenko on 17.11.2021.
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
    MockIDGenerator() {last_ = 1;};
    MOCK_METHOD(TaskID, genID, (), (override));
};

TEST_F(TaskManagerTest, shouldAddTask)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle", Task::Priority::NONE, "in 00:01", false));
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldEdit)
{
    TaskManager tm;
    TaskID id = tm.Add(Task{});
    Task t = Task::Create("My Test Task", Task::Priority::MEDIUM, "16/11/2021 19:00", false);
    tm.Edit(id, t);
    EXPECT_EQ(t, tm[id].first);
}

TEST_F(TaskManagerTest, shouldDeleteTask)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle", Task::Priority::NONE, "in 00:00", false));
    tm.Delete(id);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldCompleteTask)
{
    TaskManager tm;
    auto id = tm.Add(Task::Create("TestTitle", Task::Priority::NONE, "in 00:00", false));
    tm.Complete(id);
    EXPECT_TRUE(tm[id].first.isComplete());
}

TEST_F(TaskManagerTest, shouldThrowRuntimeErrorAtAddingTasksSameID)
{
    std::shared_ptr<MockIDGenerator> generator = std::shared_ptr<MockIDGenerator>(new MockIDGenerator);
    EXPECT_CALL(*generator, genID())
        .Times(2)
        .WillOnce(Return(TaskID(1)))
        .WillOnce(Return(TaskID(1)));

    TaskManager tm(generator);
    Task t = Task::Create("My Test Task 1", Task::Priority::MEDIUM, "30/11/2021 19:00", false);
    TaskID id = tm.Add(t);
    EXPECT_THROW(tm.Add(t), std::runtime_error);
}

TEST_F(TaskManagerTest, shouldThrowRuntimeErrorAtDeletingTaskWithChildren)
{
    TaskManager tm;
    Task::Priority p = Task::Priority::NONE;
    TaskID id = tm.Add(Task::Create("Task",p,"in 10:00",false));
    tm.Add(Task::Create("Subtask", p,"in 5:00", false), id);
    ASSERT_EQ(2, tm.size());
    EXPECT_THROW(tm.Delete(id), std::runtime_error);

}

TEST_F(TaskManagerTest, shouldDeleteAncestorsChild)
{
    TaskManager tm;
    Task::Priority p = Task::Priority::NONE;
    TaskID id1 = tm.Add(Task::Create("Task",p,"in 10:00",false));
    TaskID id2 = tm.Add(Task::Create("Subtask", p,"in 5:00", false), id1);
    ASSERT_EQ(2, tm.size());
    tm.Delete(id2);
    EXPECT_TRUE(tm[id1].second.children().empty());
}
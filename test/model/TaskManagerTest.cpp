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

class MockIDGenerator : public IDGenerator {
public:
    MockIDGenerator(int last) {last_ = last;};
    MockIDGenerator() {last_ = 1;};
    MOCK_METHOD(TaskID, genID, (), (override));
};

TEST_F(TaskManagerTest, shouldAddTask)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr) + 10,
                                    "label",
                                    false));
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldAddSubtask)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr) + 10,
                                    "label",
                                    false));
    TaskID id_ch = tm.AddSubtask(Task::Create("SubTask",
                                        Task::Priority::NONE,
                                        time(nullptr) + 5,
                                        "label",
                                        false),
                          id);
    ASSERT_EQ(2, tm.size());
    EXPECT_TRUE(tm.Validate(id));
    EXPECT_TRUE(tm.Validate(id_ch));
    EXPECT_EQ(id, tm[id_ch].second.parent());
    auto children = tm[id].second.children();
    auto it = std::find(children.begin(), children.end(), id_ch);
    EXPECT_NE(it, children.end());
}

TEST_F(TaskManagerTest, shouldChangeParent)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr) + 10,
                                    "label",
                                    false));
    TaskID id2 = tm.Add(Task::Create("TestTitle 2",
                                    Task::Priority::NONE,
                                    time(nullptr) + 100,
                                    "label",
                                    false));
    TaskID id_ch = tm.AddSubtask(Task::Create("SubTask",
                                        Task::Priority::NONE,
                                        time(nullptr) + 5,
                                        "label",
                                        false),
                          id);
    ASSERT_EQ(3, tm.size());
    tm[id_ch].second.SetParent(id2);
    tm[id].second.RemoveChild(id_ch);
    tm[id2].second.AddChild(id_ch);
    EXPECT_EQ(id2, tm[id_ch].second.parent());
    auto children = tm[id].second.children();
    EXPECT_TRUE(children.empty());
    auto children2 = tm[id2].second.children();
    EXPECT_EQ(1, children2.size());
    auto it = std::find(children2.begin(), children2.end(), id_ch);
    EXPECT_NE(it, children2.end());
}

TEST_F(TaskManagerTest, shouldRemoveAllChildren)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr) + 10,
                                    "label",
                                    false));
    tm.AddSubtask(Task::Create("Subtask",
                        Task::Priority::NONE,
                        time(nullptr) + 100,
                        "label",
                        false),
            id);
    tm.AddSubtask(Task::Create("SubTask 2",
                        Task::Priority::NONE,
                        time(nullptr) + 5,
                        "label",
                        false),
          id);
    ASSERT_EQ(3, tm.size());
    tm[id].second.RemoveChildren();
    auto children = tm[id].second.children();
    EXPECT_TRUE(children.empty());
}

TEST_F(TaskManagerTest, shouldEdit)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("My Test Task",
                                    Task::Priority::MEDIUM,
                                    time(nullptr),
                                    "label",
                                    false));
    Task t = Task::Create("Edited",
                          Task::Priority::MEDIUM,
                          time(nullptr),
                          "label",
                          false);
    tm.Edit(id, t);
    EXPECT_EQ(t, tm[id].first);
}

TEST_F(TaskManagerTest, shouldDeleteTask)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr),
                                    "label",
                                    false));
    tm.Delete(id, false);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldCompleteTask)
{
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                  Task::Priority::NONE,
                                    time(nullptr),
                                    "label",
                                    false));
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
    Task t = Task::Create("My Test Task 1",
                          Task::Priority::MEDIUM,
                          time(nullptr),
                          "label",
                          false);
    tm.Add(t);
    EXPECT_THROW(tm.Add(t), std::runtime_error);
}

TEST_F(TaskManagerTest, shouldThrowRuntimeErrorAtDeletingTaskWithChildren)
{
    TaskManager tm;
    Task::Priority p = Task::Priority::NONE;
    TaskID id = tm.Add(Task::Create("Task", p, time(nullptr), "", false));
    tm.AddSubtask(Task::Create("Subtask", p, time(nullptr), "", false), id);
    ASSERT_EQ(2, tm.size());
    EXPECT_THROW(tm.Delete(id, false), std::runtime_error);

}

TEST_F(TaskManagerTest, shouldDeleteAncestorsChild)
{
    TaskManager tm;
    Task::Priority p = Task::Priority::NONE;
    TaskID id1 = tm.Add(Task::Create("Task", p, time(nullptr), "", false));
    TaskID id2 = tm.AddSubtask(Task::Create("Subtask", p, time(nullptr), "", false), id1);
    ASSERT_EQ(2, tm.size());
    tm.Delete(id2, false);
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm[id1].second.children().empty());
}

TEST_F(TaskManagerTest, shouldAddLabel){
    TaskManager tm;
    Task::Priority p = Task::Priority::NONE;
    TaskID id = tm.Add(Task::Create("Task", p, time(nullptr), "", false));
    std::string label = "testing";
    tm.SetLabel(id, label);
    EXPECT_EQ(label, tm[id].first.label());
}

TEST_F(TaskManagerTest, shouldReturnTasks){
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr) + 10,
                                    "",
                                    false));
    TaskID id2 = tm.AddSubtask(Task::Create("Subtask",
                                    Task::Priority::NONE,
                                    time(nullptr) + 100,
                                    "",
                                    false),
                       id);
    TaskID id3 = tm.AddSubtask(Task::Create("SubTask 2",
                                    Task::Priority::NONE,
                                    time(nullptr) + 5,
                                    "",
                                    false),
                       id);

    auto tasks = tm.getTasks();
    ASSERT_EQ(3, tasks.size());
    EXPECT_EQ(tm[id].first, tasks.at(id).first);
    EXPECT_EQ(tm[id2].first, tasks.at(id2).first);
    EXPECT_EQ(tm[id3].first, tasks.at(id3).first);
}

TEST_F(TaskManagerTest, shouldReturnTasksWithSpecificLabel){
    TaskManager tm;
    TaskID id = tm.Add(Task::Create("TestTitle",
                                    Task::Priority::NONE,
                                    time(nullptr) + 10,
                                    "",
                                    false));
    TaskID id2 = tm.AddSubtask(Task::Create("Subtask",
                                     Task::Priority::NONE,
                                     time(nullptr) + 100,
                                     "",
                                     false),
                        id);
    TaskID id3 = tm.AddSubtask(Task::Create("SubTask 2",
                                     Task::Priority::NONE,
                                     time(nullptr) + 5,
                                     "",
                                     false),
                        id);

    std::string label = "testing";
    tm.SetLabel(id2, label);
    auto tasks = tm.getTasks(label);
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ(tm[id2].first, tasks.at(id2).first);
}
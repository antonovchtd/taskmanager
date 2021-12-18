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
    MOCK_METHOD(ProtoTask::TaskID, genID, (), (override));
};

TEST_F(TaskManagerTest, shouldAddTask)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldAddSubtask)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    t.set_title("SubTask");
    ProtoTask::TaskID id_ch = tm.AddSubtask(t, id);
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
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    t.set_title("TestTitle 2");
    ProtoTask::TaskID id2 = tm.Add(t);
    t.set_title("SubTask");
    ProtoTask::TaskID id_ch = tm.AddSubtask(t, id);
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
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    t.set_title("Subtask");
    tm.AddSubtask(t, id);
    t.set_title("SubTask 2");
    tm.AddSubtask(t, id);
    ASSERT_EQ(3, tm.size());
    tm[id].second.RemoveChildren();
    auto children = tm[id].second.children();
    EXPECT_TRUE(children.empty());
}

TEST_F(TaskManagerTest, shouldEdit)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    t.set_title("Edited");
    tm.Edit(id, t);
    EXPECT_EQ(t, tm[id].first);
}

TEST_F(TaskManagerTest, shouldDeleteTask)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    tm.Delete(id, false);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldCompleteTask)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    tm.Complete(id);
    EXPECT_TRUE(tm[id].first.is_complete());
}

TEST_F(TaskManagerTest, shouldThrowRuntimeErrorAtAddingTasksSameID)
{
    ProtoTask::TaskID id;
    id.set_num(1);
    std::shared_ptr<MockIDGenerator> generator = std::shared_ptr<MockIDGenerator>(new MockIDGenerator);
    EXPECT_CALL(*generator, genID())
        .Times(2)
        .WillOnce(Return(id))
        .WillOnce(Return(id));

    TaskManager tm(generator);
    ProtoTask::Task t;
    t.set_title("My Test Task 1");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_MEDIUM);
    t.set_due_date(time(nullptr));
    t.set_label("label");
    t.set_is_complete(false);
    tm.Add(t);
    EXPECT_THROW(tm.Add(t), std::runtime_error);
}

TEST_F(TaskManagerTest, shouldThrowRuntimeErrorAtDeletingTaskWithChildren)
{
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    t.set_title("Subtask");
    tm.AddSubtask(t, id);
    ASSERT_EQ(2, tm.size());
    EXPECT_THROW(tm.Delete(id, false), std::runtime_error);

}

TEST_F(TaskManagerTest, shouldDeleteAncestorsChild)
{
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id1 = tm.Add(t);
    t.set_title("Subtask");
    ProtoTask::TaskID id2 = tm.AddSubtask(t, id1);
    ASSERT_EQ(2, tm.size());
    tm.Delete(id2, false);
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm[id1].second.children().empty());
}

TEST_F(TaskManagerTest, shouldAddLabel){
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = tm.Add(t);
    std::string label = "testing";
    tm.SetLabel(id, label);
    EXPECT_EQ(label, tm[id].first.label());
}

TEST_F(TaskManagerTest, shouldReturnTasks){
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id1 = tm.Add(t);

    t.set_title("TestTitle");
    t.set_due_date(time(nullptr) + 10);
    ProtoTask::TaskID id2 = tm.AddSubtask(t, id1);

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    ProtoTask::TaskID id3 = tm.AddSubtask(t, id1);

    auto tasks = tm.getTasks();
    ASSERT_EQ(3, tasks.size());
    EXPECT_EQ(tm[id1].first, tasks.at(id1).first);
    EXPECT_EQ(tm[id2].first, tasks.at(id2).first);
    EXPECT_EQ(tm[id3].first, tasks.at(id3).first);
}

TEST_F(TaskManagerTest, shouldReturnTasksWithSpecificLabel){
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id1 = tm.Add(t);

    t.set_title("TestTitle");
    t.set_due_date(time(nullptr) + 10);
    ProtoTask::TaskID id2 = tm.AddSubtask(t, id1);

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    ProtoTask::TaskID id3 = tm.AddSubtask(t, id1);

    std::string label = "testing";
    tm.SetLabel(id2, label);
    auto tasks = tm.getTasks(label);
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ(tm[id2].first, tasks.at(id2).first);
}
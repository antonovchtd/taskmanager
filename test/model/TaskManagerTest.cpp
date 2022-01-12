//
// Created by Anton Ovcharenko on 17.11.2021.
//

#include "model/TaskManager.h"

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

TEST_F(TaskManagerTest, shouldCreateWithSpecificIDgenerator)
{
    int start_number = 42;
    auto gen = std::make_shared<IDGenerator>(start_number);
    TaskManager tm{gen};
    EXPECT_EQ(tm.gen()->state(), start_number);
}

TEST_F(TaskManagerTest, shouldCreateWithSpecificIDgeneratorAndContainer)
{
    int start_number = 42;
    auto gen = std::make_shared<IDGenerator>(start_number);
    Container con;
    con[gen->genID()] = std::make_pair(ProtoTask::Task(), Node());
    TaskManager tm{gen, con};
    EXPECT_EQ(tm.gen()->state(), start_number+1);
    EXPECT_EQ(tm.size(), 1);
    EXPECT_EQ(con, tm.getTasks());
}

TEST_F(TaskManagerTest, shouldAddTask)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = *tm.Add(t).id;
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldFailToAddTaskWithDuplicateID)
{
    auto gen = std::make_shared<MockIDGenerator>();
    ProtoTask::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    tm.Add(ProtoTask::Task());
    ActionResult result = tm.Add(ProtoTask::Task());
    EXPECT_EQ(result.status, ActionResult::Status::DUPLICATE_ID);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldFailToAddSubtaskWithDuplicateID)
{
    auto gen = std::make_shared<MockIDGenerator>();
    ProtoTask::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    tm.Add(ProtoTask::Task());
    ActionResult result = tm.AddSubtask(ProtoTask::Task(), id);
    EXPECT_EQ(result.status, ActionResult::Status::DUPLICATE_ID);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldFailToAddSubtaskWithMissingParent)
{
    auto gen = std::make_shared<MockIDGenerator>();
    ProtoTask::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    ActionResult result = tm.AddSubtask(ProtoTask::Task(), id);
    EXPECT_EQ(result.status, ActionResult::Status::PARENT_ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
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
    ProtoTask::TaskID id = *tm.Add(t).id;
    t.set_title("SubTask");
    ProtoTask::TaskID id_ch = *tm.AddSubtask(t, id).id;
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
    ProtoTask::TaskID id = *tm.Add(t).id;
    t.set_title("TestTitle 2");
    ProtoTask::TaskID id2 = *tm.Add(t).id;
    t.set_title("SubTask");
    ProtoTask::TaskID id_ch = *tm.AddSubtask(t, id).id;
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
    ProtoTask::TaskID id = *tm.Add(t).id;
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
    ProtoTask::TaskID id = *tm.Add(t).id;
    t.set_title("Edited");
    tm.Edit(id, t);
    EXPECT_EQ(t, tm[id].first);
}

TEST_F(TaskManagerTest, shouldFailToEditWrongID)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = *tm.Add(t).id;
    t.set_title("Edited");
    id.set_value(id.value()+1);
    ActionResult result = tm.Edit(id, t);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
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
    ProtoTask::TaskID id = *tm.Add(t).id;
    tm.Delete(id, false);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldFailToDeleteTaskWithWrongID)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = *tm.Add(t).id;
    id.set_value(id.value()+1);
    ActionResult result = tm.Delete(id, false);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldFailToDeleteTaskWithChildren)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    ProtoTask::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    ActionResult result = tm.Delete(id, false);
    EXPECT_EQ(result.status, ActionResult::Status::HAS_CHILDREN);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldDeleteTaskWithChildren)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    ProtoTask::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    ActionResult result = tm.Delete(id, true);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(*result.id, id);
    EXPECT_EQ(0, tm.size());
}

TEST_F(TaskManagerTest, shouldCompleteTaskWithSubtasks)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_is_complete(false);
    ProtoTask::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    tm.SetComplete(id, true);
    EXPECT_TRUE(tm[id].first.is_complete());
    auto ch_id = tm[id].second.children()[0];
    EXPECT_TRUE(tm[ch_id].first.is_complete());
}

TEST_F(TaskManagerTest, shouldFailToCompleteTaskWithWrongID)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(false);
    ProtoTask::TaskID id = *tm.Add(t).id;
    id.set_value(id.value()+1);
    ActionResult result = tm.SetComplete(id, true);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldUncompleteTask)
{
    TaskManager tm;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(ProtoTask::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.set_label("label");
    t.set_is_complete(true);
    ProtoTask::TaskID id = *tm.Add(t).id;
    tm.SetComplete(id, false);
    EXPECT_FALSE(tm[id].first.is_complete());
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
    ProtoTask::TaskID id1 = *tm.Add(t).id;
    t.set_title("Subtask");
    ProtoTask::TaskID id2 = *tm.AddSubtask(t, id1).id;
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
    ProtoTask::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    tm.AddLabel(id, label);
    EXPECT_EQ(label, tm[id].first.label());
}

TEST_F(TaskManagerTest, shouldFailToAddLabelWithWrongID){
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    id.set_value(id.value()+1);
    ActionResult result = tm.AddLabel(id, label);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
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
    ProtoTask::TaskID id1 = *tm.Add(t).id;

    t.set_title("TestTitle");
    t.set_due_date(time(nullptr) + 10);
    ProtoTask::TaskID id2 = *tm.AddSubtask(t, id1).id;

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    ProtoTask::TaskID id3 = *tm.AddSubtask(t, id1).id;

    auto tasks = tm.getTasks();
    ASSERT_EQ(3, tasks.size());
    EXPECT_EQ(tm[id1].first, tasks.at(id1).first);
    EXPECT_EQ(tm[id2].first, tasks.at(id2).first);
    EXPECT_EQ(tm[id3].first, tasks.at(id3).first);
}

TEST_F(TaskManagerTest, shouldReturnTasksWithSpecificID){
    TaskManager tm;
    ProtoTask::Task::Priority p = ProtoTask::Task::Priority::Task_Priority_NONE;
    ProtoTask::Task t;
    t.set_title("TestTitle");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_label("");
    t.set_is_complete(false);
    ProtoTask::TaskID id1 = *tm.Add(t).id;

    t.set_title("TestTitle");
    t.set_due_date(time(nullptr) + 10);
    ProtoTask::TaskID id2 = *tm.AddSubtask(t, id1).id;

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    ProtoTask::TaskID id3 = *tm.AddSubtask(t, id2).id;

    auto tasks = tm.getTasks(id2);
    ASSERT_EQ(2, tasks.size());
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
    ProtoTask::TaskID id1 = *tm.Add(t).id;

    t.set_title("TestTitle");
    t.set_due_date(time(nullptr) + 10);
    ProtoTask::TaskID id2 = *tm.AddSubtask(t, id1).id;

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    ProtoTask::TaskID id3 = *tm.AddSubtask(t, id1).id;

    std::string label = "testing";
    tm.AddLabel(id2, label);
    auto tasks = tm.getTasks(label);
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ(tm[id2].first, tasks.at(id2).first);
}

TEST_F(TaskManagerTest, shouldExportTasks)
{
    int start_number = 42;
    auto gen = std::make_shared<IDGenerator>(start_number);
    Container con;
    ProtoTask::Task parent, child;
    parent.set_title("parent title");
    child.set_title("child title");
    auto parent_id = gen->genID();
    con[parent_id] = std::make_pair(parent, Node());
    auto child_id = gen->genID();
    con[child_id] = std::make_pair(child, Node(parent_id));
    TaskManager tm{gen, con};
    auto vec = tm.getTasks();
    EXPECT_EQ(vec[0].id(), parent_id);
    EXPECT_EQ(vec[0].data(), parent);
    EXPECT_FALSE(vec[0].has_parent());
    EXPECT_EQ(vec[1].id(), child_id);
    EXPECT_EQ(vec[1].data(), child);
    ASSERT_TRUE(vec[1].has_parent());
    EXPECT_EQ(vec[1].parent(), parent_id);
}

TEST_F(TaskManagerTest, shouldReplaceTasks)
{
    std::vector<ProtoTask::TaskEntity> vec;
    ProtoTask::TaskEntity te, te2;
    auto parent = new ProtoTask::Task;
    parent->set_title("parent title");
    auto child = new ProtoTask::Task;
    child->set_title("child title");
    auto id = new ProtoTask::TaskID;
    id->set_value(1);
    auto parent_id = new ProtoTask::TaskID;
    parent_id->set_value(1);
    auto child_id = new ProtoTask::TaskID;
    child_id->set_value(2);
    te.set_allocated_id(id);
    te.set_allocated_data(parent);
    vec.push_back(te);
    te2.set_allocated_id(child_id);
    te2.set_allocated_data(child);
    te2.set_allocated_parent(parent_id);
    vec.push_back(te2);
    TaskManager tm;
    tm.Replace(vec);
    ASSERT_EQ(tm.size(), vec.size());
    EXPECT_EQ(tm[*id].first, *parent);
    EXPECT_EQ(tm[*child_id].first, *child);
    EXPECT_EQ(tm[*child_id].second.parent(), *parent_id);
}
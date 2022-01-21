//
// Created by Anton Ovcharenko on 17.11.2021.
//

#include "model/TaskManager.h"
#include "utilities/TaskEntityUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;

class TaskManagerTest : public ::testing::Test
{

};

class MockIDGenerator : public IDGenerator {
public:
    explicit MockIDGenerator(int last) {last_ = last;};
    MockIDGenerator() {last_ = 1;};
    MOCK_METHOD(Core::TaskID, genID, (), (override));
};

TEST_F(TaskManagerTest, shouldCreateWithSpecificIDgenerator)
{
    int start_number = 42;
    auto gen = std::make_shared<IDGenerator>(start_number);
    TaskManager tm{gen};
    EXPECT_EQ(tm.gen()->state(), start_number);
}

TEST_F(TaskManagerTest, shouldAddTask)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldFailToAddTaskWithDuplicateID)
{
    auto gen = std::make_shared<MockIDGenerator>();
    Core::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    tm.Add(Core::Task());
    ActionResult result = tm.Add(Core::Task());
    EXPECT_EQ(result.status, ActionResult::Status::DUPLICATE_ID);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldFailToAddSubtaskWithDuplicateID)
{
    auto gen = std::make_shared<MockIDGenerator>();
    Core::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    tm.Add(Core::Task());
    ActionResult result = tm.AddSubtask(Core::Task(), id);
    EXPECT_EQ(result.status, ActionResult::Status::DUPLICATE_ID);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldFailToAddSubtaskWithMissingParent)
{
    auto gen = std::make_shared<MockIDGenerator>();
    Core::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    ActionResult result = tm.AddSubtask(Core::Task(), id);
    EXPECT_EQ(result.status, ActionResult::Status::PARENT_ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldAddSubtask)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    t.set_title("SubTask");
    Core::TaskID id_ch = *tm.AddSubtask(t, id).id;
    ASSERT_EQ(2, tm.size());
    EXPECT_TRUE(tm.Validate(id));
    EXPECT_TRUE(tm.Validate(id_ch));
    EXPECT_EQ(id, tm.getTasks()[1].parent());
}

TEST_F(TaskManagerTest, shouldEdit)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    t.set_title("Edited");
    tm.Edit(id, t);
    EXPECT_EQ(t, tm.getTasks()[0].data());
}

TEST_F(TaskManagerTest, shouldFailToEditWrongID)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    t.set_title("Edited");
    id.set_value(id.value()+1);
    ActionResult result = tm.Edit(id, t);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldDeleteTask)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    tm.Delete(id, false);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(TaskManagerTest, shouldFailToDeleteTaskWithWrongID)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    id.set_value(id.value()+1);
    ActionResult result = tm.Delete(id, false);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldFailToDeleteTaskWithChildren)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    Core::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    ActionResult result = tm.Delete(id, false);
    EXPECT_EQ(result.status, ActionResult::Status::HAS_CHILDREN);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldDeleteTaskWithChildren)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    Core::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    ActionResult result = tm.Delete(id, true);
    EXPECT_EQ(result.status, ActionResult::Status::SUCCESS);
    EXPECT_EQ(*result.id, id);
    EXPECT_EQ(0, tm.size());
}

TEST_F(TaskManagerTest, shouldCompleteTaskWithSubtasks)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    tm.Complete(id);
    auto tasks = tm.getTasks();
    EXPECT_TRUE(tasks[0].data().is_complete());
    EXPECT_TRUE(tasks[1].data().is_complete());
    EXPECT_EQ(tasks[1].parent(), id);
}

TEST_F(TaskManagerTest, shouldFailToCompleteTaskWithWrongID)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    id.set_value(id.value()+1);
    ActionResult result = tm.Complete(id);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldUncompleteTask)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(Core::Task::Priority::Task_Priority_NONE);
    t.set_due_date(time(nullptr) + 10);
    t.add_labels("label");
    t.set_is_complete(true);
    Core::TaskID id = *tm.Add(t).id;
    tm.AddSubtask(t, id);
    tm.Uncomplete(id);
    auto tasks = tm.getTasks();
    ASSERT_EQ(2, tasks.size());
    EXPECT_FALSE(tasks[0].data().is_complete());
    EXPECT_FALSE(tasks[1].data().is_complete());
}

TEST_F(TaskManagerTest, shouldFailToUncompleteTaskWithWrongID)
{
    TaskManager tm;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_is_complete(true);
    Core::TaskID id = *tm.Add(t).id;
    id.set_value(2);
    ActionResult result = tm.Uncomplete(id);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldDeleteAncestorsChild)
{
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id1 = *tm.Add(t).id;
    t.set_title("Subtask");
    Core::TaskID id2 = *tm.AddSubtask(t, id1).id;
    ASSERT_EQ(2, tm.size());
    tm.Delete(id2, false);
    ASSERT_EQ(1, tm.size());
    EXPECT_EQ(tm.getTasks()[0].id(), id1);
}

TEST_F(TaskManagerTest, shouldAddLabel){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    tm.AddLabel(id, label);
    EXPECT_EQ(label, tm.getTasks()[0].data().labels()[0]);
}

TEST_F(TaskManagerTest, shouldAddMultipleLabels){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    std::string label2 = "testing2";
    tm.AddLabel(id, label);
    tm.AddLabel(id, label2);
    EXPECT_EQ(label, tm.getTasks()[0].data().labels()[0]);
    EXPECT_EQ(label2, tm.getTasks()[0].data().labels()[1]);
}

TEST_F(TaskManagerTest, shouldAddMultipleLabelsClearOne){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    std::string label2 = "testing2";
    tm.AddLabel(id, label);
    tm.AddLabel(id, label2);
    tm.ClearLabel(id, label);
    auto tasks = tm.getTasks();
    EXPECT_EQ(1, tasks[0].data().labels().size());
    EXPECT_EQ(label2, tasks[0].data().labels()[0]);
}

TEST_F(TaskManagerTest, shouldFailToClearLabelInvalidID){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    tm.AddLabel(id, label);
    Core::TaskID id2;
    id2.set_value(id.value()+1);
    ActionResult result = tm.ClearLabel(id2, label);
    auto tasks = tm.getTasks();
    EXPECT_EQ(1, tasks[0].data().labels().size());
    EXPECT_EQ(label, tasks[0].data().labels()[0]);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id2);
}

TEST_F(TaskManagerTest, shouldFailToClearAllLabelsInvalidID){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    tm.AddLabel(id, label);
    Core::TaskID id2;
    id2.set_value(id.value()+1);
    ActionResult result = tm.ClearLabels(id2);
    auto tasks = tm.getTasks();
    EXPECT_EQ(1, tasks[0].data().labels().size());
    EXPECT_EQ(label, tasks[0].data().labels()[0]);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id2);
}

TEST_F(TaskManagerTest, shouldAddMultipleLabelsClearAll){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    std::string label2 = "testing2";
    tm.AddLabel(id, label);
    tm.AddLabel(id, label2);
    tm.ClearLabels(id);
    EXPECT_TRUE(tm.getTasks()[0].data().labels().empty());
}

TEST_F(TaskManagerTest, shouldFailToAddLabelWithWrongID){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("Task");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id = *tm.Add(t).id;
    std::string label = "testing";
    id.set_value(id.value()+1);
    ActionResult result = tm.AddLabel(id, label);
    EXPECT_EQ(result.status, ActionResult::Status::ID_NOT_FOUND);
    EXPECT_EQ(*result.id, id);
}

TEST_F(TaskManagerTest, shouldReturnTasks){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id1 = *tm.Add(t).id;

    t.set_title("SubTask 1");
    t.set_due_date(time(nullptr) + 10);
    Core::TaskID id2 = *tm.AddSubtask(t, id1).id;

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    Core::TaskID id3 = *tm.AddSubtask(t, id2).id;

    auto tasks = tm.getTasks();
    ASSERT_EQ(3, tasks.size());
    EXPECT_EQ("TestTitle", tasks[0].data().title());
    EXPECT_EQ(id1, tasks[0].id());
    EXPECT_FALSE(tasks[0].has_parent());
    EXPECT_EQ("SubTask 1", tasks[1].data().title());
    EXPECT_EQ(id2, tasks[1].id());
    EXPECT_EQ(id1, tasks[1].parent());
    EXPECT_EQ("SubTask 2", tasks[2].data().title());
    EXPECT_EQ(id3, tasks[2].id());
    EXPECT_EQ(id2, tasks[2].parent());
}

TEST_F(TaskManagerTest, shouldReturnTasksWithSpecificID){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id1 = *tm.Add(t).id;

    t.set_title("SubTask 1");
    t.set_due_date(time(nullptr) + 10);
    Core::TaskID id2 = *tm.AddSubtask(t, id1).id;

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    Core::TaskID id3 = *tm.AddSubtask(t, id2).id;

    auto tasks = tm.getTaskWithSubtasks(id2);
    ASSERT_EQ(2, tasks.size());
    EXPECT_EQ("SubTask 1", tasks[0].data().title());
    EXPECT_EQ(id2, tasks[0].id());
    EXPECT_FALSE(tasks[0].has_parent());
    EXPECT_EQ("SubTask 2", tasks[1].data().title());
    EXPECT_EQ(id3, tasks[1].id());
    EXPECT_EQ(id2, tasks[1].parent());
}

TEST_F(TaskManagerTest, shouldReturnTasksWithSpecificLabel){
    TaskManager tm;
    Core::Task::Priority p = Core::Task::Priority::Task_Priority_NONE;
    Core::Task t;
    t.set_title("TestTitle");
    t.set_priority(p);
    t.set_due_date(time(nullptr));
    t.set_is_complete(false);
    Core::TaskID id1 = *tm.Add(t).id;

    t.set_title("SubTask 1");
    t.set_due_date(time(nullptr) + 10);
    Core::TaskID id2 = *tm.AddSubtask(t, id1).id;

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    Core::TaskID id3 = *tm.AddSubtask(t, id2).id;

    std::string label = "testing";
    tm.AddLabel(id2, label);
    auto tasks = tm.getTasks(label);
    ASSERT_EQ(1, tasks.size());
    EXPECT_EQ(id2, tasks[0].id());
    EXPECT_EQ("SubTask 1", tasks[0].data().title());
}

TEST_F(TaskManagerTest, shouldReplaceTasks)
{
    std::vector<Core::TaskEntity> vec;
    Core::TaskEntity te, te2;
    auto parent = new Core::Task;
    parent->set_title("parent title");
    auto child = new Core::Task;
    child->set_title("child title");
    auto id = new Core::TaskID;
    id->set_value(1);
    auto parent_id = new Core::TaskID;
    parent_id->set_value(1);
    auto child_id = new Core::TaskID;
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
    auto tasks = tm.getTasks();
    ASSERT_EQ(tasks.size(), vec.size());
    for (int i = 0; i<tasks.size(); ++i)
        EXPECT_EQ(tasks[i], vec[i]);
}
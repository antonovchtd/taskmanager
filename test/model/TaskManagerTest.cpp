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
public:
    Core::Task task_;

    void SetUp() override {
        task_ = Core::createTask("TestTitle",
                                 Core::Task::Priority::Task_Priority_NONE,
                                 time(nullptr) + 10,
                                 "",
                                 false);
    }
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
    Core::ModelRequestResult result = tm.Add(task_);
    ASSERT_TRUE(result.has_id());
    const Core::TaskID& id = result.id();
    ASSERT_EQ(1, tm.size());
    EXPECT_TRUE(ToBool(tm.IsPresent(id)));
}

TEST_F(TaskManagerTest, shouldFailToAddSubtaskWithMissingParent)
{
    auto gen = std::make_shared<MockIDGenerator>();
    Core::TaskID id;
    id.set_value(1);
    EXPECT_CALL(*gen, genID).WillRepeatedly(Return(id));
    TaskManager tm{gen};
    Core::ModelRequestResult result = tm.AddSubtask(Core::Task(), id);

    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(result.status(), Core::ModelRequestResult_Status_PARENT_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldAddSubtask)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();
    task_.set_title("SubTask");
    Core::ModelRequestResult subtask_result = tm.AddSubtask(task_, id);
    ASSERT_TRUE(subtask_result.has_id());
    const auto& id_ch = subtask_result.id();

    ASSERT_EQ(2, tm.size());
    EXPECT_TRUE(ToBool(tm.IsPresent(id)));
    EXPECT_TRUE(ToBool(tm.IsPresent(id_ch)));
    EXPECT_EQ(id, tm.getTasks()[1].parent());
}

TEST_F(TaskManagerTest, shouldEdit)
{
    TaskManager tm;
    Core::ModelRequestResult result = tm.Add(task_);
    ASSERT_TRUE(result.has_id());
    const auto& id = result.id();

    task_.set_title("Edited");
    tm.Edit(id, task_);
    EXPECT_EQ(task_, tm.getTasks()[0].data());
}

TEST_F(TaskManagerTest, shouldFailToEditWrongID)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    auto id = add_result.id();

    task_.set_title("Edited");
    id.set_value(id.value()+1);
    Core::ModelRequestResult edit_result = tm.Edit(id, task_);
    ASSERT_TRUE(edit_result.has_status());
    EXPECT_EQ(edit_result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldDeleteTask)
{
    TaskManager tm;
    Core::ModelRequestResult result = tm.Add(task_);
    ASSERT_TRUE(result.has_id());
    const auto& id = result.id();
    tm.Delete(id, false);
    EXPECT_FALSE(ToBool(tm.IsPresent(id)));
}

TEST_F(TaskManagerTest, shouldFailToDeleteTaskWithWrongID)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    auto id = add_result.id();

    id.set_value(id.value()+1);
    Core::ModelRequestResult delete_result = tm.Delete(id, false);
    ASSERT_TRUE(delete_result.has_status());
    EXPECT_EQ(delete_result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldFailToDeleteTaskWithChildren)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();
    tm.AddSubtask(task_, id);
    Core::ModelRequestResult delete_result = tm.Delete(id, false);
    ASSERT_TRUE(delete_result.has_status());
    EXPECT_EQ(delete_result.status(), Core::ModelRequestResult_Status_HAS_CHILDREN);
}

TEST_F(TaskManagerTest, shouldDeleteTaskWithChildren)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    tm.AddSubtask(task_, id);
    Core::ModelRequestResult delete_result = tm.Delete(id, true);
    ASSERT_TRUE(delete_result.has_id());
    EXPECT_EQ(delete_result.id(), id);
    EXPECT_EQ(0, tm.size());
}

TEST_F(TaskManagerTest, shouldCompleteTaskWithSubtasks)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    tm.AddSubtask(task_, id);
    tm.Complete(id);
    auto tasks = tm.getTasks();
    EXPECT_TRUE(tasks[0].data().is_complete());
    EXPECT_TRUE(tasks[1].data().is_complete());
    EXPECT_EQ(tasks[1].parent(), id);
}

TEST_F(TaskManagerTest, shouldFailToCompleteTaskWithWrongID)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    auto id = add_result.id();

    id.set_value(id.value()+1);
    Core::ModelRequestResult result = tm.Complete(id);
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldUncompleteTask)
{
    TaskManager tm;
    task_.set_is_complete(true);

    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    tm.AddSubtask(task_, id);
    tm.Uncomplete(id);
    auto tasks = tm.getTasks();
    ASSERT_EQ(2, tasks.size());
    EXPECT_FALSE(tasks[0].data().is_complete());
    EXPECT_FALSE(tasks[1].data().is_complete());
}

TEST_F(TaskManagerTest, shouldFailToUncompleteTaskWithWrongID)
{
    TaskManager tm;
    task_.set_is_complete(true);

    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    auto id = add_result.id();

    id.set_value(2);
    Core::ModelRequestResult result = tm.Uncomplete(id);
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldDeleteAncestorsChild)
{
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id1 = add_result.id();

    task_.set_title("Subtask");
    Core::ModelRequestResult subtask_result = tm.AddSubtask(task_, id1);
    ASSERT_TRUE(subtask_result.has_id());
    const auto& id2 = subtask_result.id();

    ASSERT_EQ(2, tm.size());
    tm.Delete(id2, true);
    ASSERT_EQ(1, tm.size());
    EXPECT_EQ(tm.getTasks()[0].id(), id1);
}

TEST_F(TaskManagerTest, shouldAddLabel){
    TaskManager tm;

    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    Core::Label label;
    label.set_str("testing");
    tm.AddLabel(id, label);
    EXPECT_EQ(label, tm.getTasks()[0].data().labels()[0]);
}

TEST_F(TaskManagerTest, shouldAddMultipleLabels){
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    Core::Label label, label2;
    label.set_str("testing");
    label2.set_str("testing2");
    tm.AddLabel(id, label);
    tm.AddLabel(id, label2);
    EXPECT_EQ(label, tm.getTasks()[0].data().labels()[0]);
    EXPECT_EQ(label2, tm.getTasks()[0].data().labels()[1]);
}

TEST_F(TaskManagerTest, shouldAddMultipleLabelsClearOne){
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    Core::Label label, label2;
    label.set_str("testing");
    label2.set_str("testing2");
    tm.AddLabel(id, label);
    tm.AddLabel(id, label2);
    tm.RemoveLabel(id, label);
    auto tasks = tm.getTasks();
    ASSERT_EQ(1, tasks[0].data().labels().size());
    EXPECT_EQ(label2, tasks[0].data().labels()[0]);
}

TEST_F(TaskManagerTest, shouldFailToClearLabelInvalidID){
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    Core::Label label;
    label.set_str("testing");
    tm.AddLabel(id, label);
    Core::TaskID id2;
    id2.set_value(id.value()+1);
    Core::ModelRequestResult clear_result = tm.RemoveLabel(id2, label);
    auto tasks = tm.getTasks();
    ASSERT_EQ(1, tasks[0].data().labels().size());
    ASSERT_TRUE(clear_result.has_status());
    EXPECT_EQ(clear_result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldFailToClearAllLabelsInvalidID){
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    Core::Label label;
    label.set_str("testing");
    tm.AddLabel(id, label);
    Core::TaskID id2;
    id2.set_value(id.value()+1);
    Core::ModelRequestResult clear_result = tm.RemoveAllLabels(id2);
    auto tasks = tm.getTasks();
    ASSERT_EQ(1, tasks[0].data().labels().size());
    EXPECT_EQ(label, tasks[0].data().labels()[0]);
    ASSERT_TRUE(clear_result.has_status());
    EXPECT_EQ(clear_result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldAddMultipleLabelsClearAll){
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    const auto& id = add_result.id();

    Core::Label label, label2;
    label.set_str("testing");
    label2.set_str("testing2");
    tm.AddLabel(id, label);
    tm.AddLabel(id, label2);
    tm.RemoveAllLabels(id);
    EXPECT_TRUE(tm.getTasks()[0].data().labels().empty());
}

TEST_F(TaskManagerTest, shouldFailToAddLabelWithWrongID){
    TaskManager tm;

    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    auto id = add_result.id();

    Core::Label label;
    label.set_str("testing");
    id.set_value(id.value()+1);
    Core::ModelRequestResult result = tm.AddLabel(id, label);
    ASSERT_TRUE(result.has_status());
    EXPECT_EQ(result.status(), Core::ModelRequestResult_Status_ID_NOT_FOUND);
}

TEST_F(TaskManagerTest, shouldReturnTasks){
    TaskManager tm;
    Core::ModelRequestResult add_result = tm.Add(task_);
    ASSERT_TRUE(add_result.has_id());
    auto id1 = add_result.id();

    task_.set_title("SubTask 1");
    task_.set_due_date(time(nullptr) + 10);

    add_result = tm.AddSubtask(task_, id1);
    ASSERT_TRUE(add_result.has_id());
    auto id2 = add_result.id();

    task_.set_title("SubTask 2");
    task_.set_due_date(time(nullptr) + 5);

    add_result = tm.AddSubtask(task_, id2);
    ASSERT_TRUE(add_result.has_id());
    auto id3 = add_result.id();

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

    Core::ModelRequestResult add_result = tm.Add(t);
    ASSERT_TRUE(add_result.has_id());
    auto id1 = add_result.id();

    t.set_title("SubTask 1");
    t.set_due_date(time(nullptr) + 10);

    add_result = tm.AddSubtask(t, id1);
    ASSERT_TRUE(add_result.has_id());
    auto id2 = add_result.id();

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    add_result = tm.AddSubtask(t, id2);
    ASSERT_TRUE(add_result.has_id());
    auto id3 = add_result.id();

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

    Core::ModelRequestResult add_result = tm.Add(t);
    ASSERT_TRUE(add_result.has_id());
    auto id1 = add_result.id();

    t.set_title("SubTask 1");
    t.set_due_date(time(nullptr) + 10);
    add_result = tm.AddSubtask(t, id1);
    ASSERT_TRUE(add_result.has_id());
    auto id2 = add_result.id();

    t.set_title("SubTask 2");
    t.set_due_date(time(nullptr) + 5);
    add_result = tm.AddSubtask(t, id2);
    ASSERT_TRUE(add_result.has_id());

    Core::Label label;
    label.set_str("testing");
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
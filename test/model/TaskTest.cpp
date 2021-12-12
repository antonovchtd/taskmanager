//
// Created by Anton O. on 11/12/21.
//

#include "../../src/model/Task.h"
#include <ctime>

#include <gtest/gtest.h>

class TaskTest : public ::testing::Test
{

};

TEST_F(TaskTest, shouldCreate)
{
    const auto title = "MyTask";
    auto p = Task::Priority::HIGH;
    bool completeFlag = true;
    time_t currtime = time(nullptr);
    std::string label = "test";
    Task t = Task::Create(title, p, currtime, label, completeFlag);
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(p, t.priority());
    EXPECT_EQ(currtime, t.dueDate());
    EXPECT_EQ(label, t.label());
    EXPECT_EQ(completeFlag, t.isComplete());
}

TEST_F(TaskTest, shouldCreateFromData)
{
    const auto title = "MyTask";
    auto p = Task::Priority::HIGH;
    bool completeFlag = true;
    time_t currtime = time(nullptr);
    std::string label = "test";
    Task::Data data = {title, p, currtime, label, true};
    Task t = Task::Create(data);
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(p, t.priority());
    EXPECT_EQ(currtime, t.dueDate());
    EXPECT_EQ(label, t.label());
    EXPECT_EQ(completeFlag, t.isComplete());
    EXPECT_EQ(t, Task::Create(t.data()));
}

TEST_F(TaskTest, shouldCreateAtSpecificDate)
{
    struct tm timeinfo{};
    timeinfo.tm_year = 121;
    timeinfo.tm_mon = 10;
    timeinfo.tm_mday = 16;
    timeinfo.tm_hour = 19;
    time_t test_time = mktime(&timeinfo);

    Task t = Task::Create("TestTask",Task::Priority::NONE, test_time, "", false);
    EXPECT_EQ(test_time, t.dueDate());
}

TEST_F(TaskTest, shouldThrowRuntimeErrorAtEmptyTitle)
{
    EXPECT_THROW(Task::Create("", Task::Priority::HIGH, 0, "", false),
                 std::runtime_error);
}



//
// Created by Anton O. on 11/12/21.
//

#include "../../src/model/Task.h"

#include <gtest/gtest.h>

class TaskTest : public ::testing::Test
{

};

TEST_F(TaskTest, shouldCreate)
{
    const auto title = "MyTask";
    auto p = Task::Priority::HIGH;
    Task t = Task::Create(title, p, 0);
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(p, t.priority());

    t = Task::Create(title, 0);
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(Task::Priority::NONE, t.priority());

    t = Task::Create(title, p);
    EXPECT_EQ(title, t.title());
    EXPECT_EQ(p, t.priority());
}

TEST_F(TaskTest, shouldCreateAtSpecificDate)
{
    struct tm timeinfo{};
    timeinfo.tm_year = 121;
    timeinfo.tm_mon = 10;
    timeinfo.tm_mday = 16;
    timeinfo.tm_hour = 19;

    Task t = Task::Create("TestTask","16/11/21 19:00");
    EXPECT_EQ(mktime(&timeinfo), t.due_date());

    t = Task::Create("TestTask","16/11/2021 19:00");
    EXPECT_EQ(mktime(&timeinfo), t.due_date());

    timeinfo.tm_hour = 0;
    t = Task::Create("TestTask","16/11/2021");
    EXPECT_EQ(mktime(&timeinfo), t.due_date());
}

TEST_F(TaskTest, shouldCreateAtFutureDate)
{
    time_t rawtime;
    time(&rawtime);
    rawtime += 2*24*3600 + 10*3600 + 5*60;
    Task t = Task::Create("TestTask",Task::Priority::NONE,"in 02:10:05");
    EXPECT_EQ(rawtime, t.due_date());

    time(&rawtime);
    rawtime += 10*3600 + 5*60;
    t = Task::Create("TestTask","in 10:05");
    EXPECT_EQ(rawtime, t.due_date());
}

TEST_F(TaskTest, shouldThrowRuntimeErrorAtEmptyTitle)
{
    EXPECT_THROW(Task::Create("", Task::Priority::HIGH, 0), std::runtime_error);
}

TEST_F(TaskTest, shouldSetComplete)
{
    bool status = true;
    Task t = Task::Create("TestTitle");
    t.setComplete(status);
    EXPECT_EQ(status, t.isComplete());
}


//
// Created by Anton O. on 12/16/21.
//

#include "ui/Machine.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class PersisterTest : public ::testing::Test
{

};

class MockReader : public AbstractReader {
public:
    MOCK_METHOD(std::string, read, (const std::string &prompt), (override));
};

class MockPrinter : public AbstractPrinter {
public:
    MOCK_METHOD(void, print, (const std::string &message), (override));
};

TEST_F(PersisterTest, shouldSaveAndLoadTasksMapWithGeneratorState)
{
    auto tm = std::make_shared<TaskManager>();
    Core::Task t = Core::createTask("task 1", Core::Task_Priority_HIGH,
                                    time(nullptr), "a", false);
    Core::Task sub = Core::createTask("subtask 2", Core::Task_Priority_MEDIUM,
                                      time(nullptr), "b", false);
    Core::Task subsub = Core::createTask("subsubtask 3", Core::Task_Priority_LOW,
                                         time(nullptr), "c", false);
    auto id1 = *tm->Add(t).id;
    auto id2 = *tm->AddSubtask(sub, id1).id;
    auto id3 = *tm->AddSubtask(subsub, id2).id;

    std::string filename = "PersisterTest.bin";
    FilePersistence p;
    p.save(filename, tm);
    auto tm_loaded = std::make_shared<TaskManager>();
    p.load(filename, tm_loaded);

    ASSERT_EQ(tm->size(), tm_loaded->size());
    for (const auto &kv : tm->getTasks()) {
        EXPECT_EQ(kv.second.first, (*tm_loaded)[kv.first].first);
        EXPECT_EQ(kv.second.second, (*tm_loaded)[kv.first].second);
    }
    EXPECT_EQ(tm->gen()->state(), tm_loaded->gen()->state());
}

TEST_F(PersisterTest, shouldFailedToLoadNonExistingFile)
{
    auto tm = std::make_shared<TaskManager>();
    std::string filename = "Missing.bin";
    FilePersistence p;
    EXPECT_FALSE(p.load(filename, tm));
}
//
// Created by Anton O. on 12/16/21.
//

#include "../../src/ui/Machine.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class PersistorTest : public ::testing::Test
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

TEST_F(PersistorTest, shouldSaveAndLoadTasksMapWithGeneratorState)
{
    TaskManager tm;
    ProtoTask::Task t = ProtoTask::create("task 1", ProtoTask::Task_Priority_HIGH,
                                          time(nullptr), "a", false);
    ProtoTask::Task sub = ProtoTask::create("subtask 2", ProtoTask::Task_Priority_MEDIUM,
                                          time(nullptr), "b", false);
    ProtoTask::Task subsub = ProtoTask::create("subsubtask 3", ProtoTask::Task_Priority_LOW,
                                          time(nullptr), "c", false);
    auto id1 = tm.Add(t);
    auto id2 = tm.AddSubtask(sub, id1);
    auto id3 = tm.AddSubtask(subsub, id2);

    std::string filename = "PersistorTest.bin";
    std::ofstream ofile(filename, std::ios::trunc | std::ios::binary);
    Persistor::save(tm, ofile);
    ofile.close();
    std::ifstream ifile(filename, std::ios::binary);
    auto tm_loaded = Persistor::load(ifile);
    ifile.close();

    ASSERT_EQ(tm.size(), tm_loaded.size());
    for (const auto &kv : tm.getTasks()) {
        EXPECT_EQ(kv.second.first, tm_loaded[kv.first].first);
        EXPECT_EQ(kv.second.second, tm_loaded[kv.first].second);
    }
}
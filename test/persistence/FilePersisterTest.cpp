//
// Created by Anton O. on 12/16/21.
//

#include "persistence/FilePersistence.h"
#include "utilities/TaskUtils.h"
#include "utilities/TaskEntityUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class FilePersisterTest : public ::testing::Test
{

};

TEST_F(FilePersisterTest, shouldSaveAndLoadTasks)
{
    std::vector<Core::TaskEntity> vec;

    Core::Task t = Core::createTask("task 1", Core::Task_Priority_HIGH,
                                    time(nullptr), "a", false);
    Core::Task sub = Core::createTask("subtask 2", Core::Task_Priority_MEDIUM,
                                      time(nullptr), "b", false);
    Core::Task subsub = Core::createTask("subsubtask 3", Core::Task_Priority_LOW,
                                         time(nullptr), "c", false);
    Core::TaskEntity te;
    Core::TaskID id;
    id.set_value(1);
    te.set_allocated_id(new Core::TaskID(id));
    te.set_allocated_data(new Core::Task(t));
    vec.push_back(te);

    te.set_allocated_parent(new Core::TaskID(id));
    id.set_value(2);
    te.set_allocated_id(new Core::TaskID(id));
    te.set_allocated_data(new Core::Task(sub));
    vec.push_back(te);

    te.set_allocated_parent(new Core::TaskID(id));
    id.set_value(3);
    te.set_allocated_id(new Core::TaskID(id));
    te.set_allocated_data(new Core::Task(subsub));
    vec.push_back(te);

    FilePersistence p{"PersisterTest.bin"};
    ASSERT_TRUE(p.save(vec));
    std::vector<Core::TaskEntity> vec2;
    ASSERT_TRUE(p.load(vec2));

    ASSERT_EQ(vec.size(), vec2.size());
    for (int i = 0; i<vec.size(); ++i) {
        EXPECT_EQ(vec[i], vec2[i]);
    }
}

TEST_F(FilePersisterTest, shouldFailedToLoadNonExistingFile)
{
    std::string filename = "Missing.bin";
    FilePersistence p{filename};
    std::vector<Core::TaskEntity> vec;
    EXPECT_FALSE(p.load(vec));
}
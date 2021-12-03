//
// Created by Anton O. on 12/3/21.
//

#include "../../src/model/TaskID.h"

#include <gtest/gtest.h>

class TaskIDTest : public ::testing::Test
{

};

TEST_F(TaskIDTest, shouldCreateFromString) {
    std::string str = "123";
    TaskID id = TaskID::Create(str);
    EXPECT_EQ(123, id.value());
    EXPECT_NE(1, id.value());
    EXPECT_EQ(str, id.to_string());
}

TEST_F(TaskIDTest, shouldCreateInvalidIDFromBadString) {
    std::string str = "bad";
    TaskID id = TaskID::Create(str);
    EXPECT_FALSE(id.isValid());
}

TEST_F(TaskIDTest, shouldCreateInvalidIDFromEmptyString) {
    std::string str = "";
    TaskID id = TaskID::Create(str);
    EXPECT_FALSE(id.isValid());
}

TEST_F(TaskIDTest, shouldCreateNullID) {
    TaskID id = TaskID::nullid();
    EXPECT_EQ(0, id.value());
}
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

TEST_F(PersistorTest, shouldSaveAndLoadTask)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(17)
            .WillOnce(Return("add"))
            .WillOnce(Return("Task 1"))
            .WillOnce(Return("1"))
            .WillOnce(Return("21/12"))
            .WillOnce(Return("subtask 1"))
            .WillOnce(Return("Subtask 2"))
            .WillOnce(Return("2"))
            .WillOnce(Return("22/12"))
            .WillOnce(Return("subtask 2"))
            .WillOnce(Return("Subtask 3"))
            .WillOnce(Return("3"))
            .WillOnce(Return("23/12"))
            .WillOnce(Return("complete 1"))
            .WillOnce(Return("save"))
            .WillOnce(Return("quit"))
            .WillOnce(Return("load"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    Machine m2(f, Factory::State::HOME);
    m2.run();
    TaskManager tm = *m2.model();
    ASSERT_EQ(3, tm.size());

    ProtoTask::TaskID id1, id2, id3;
    id1.set_num(1);
    id2.set_num(2);
    id3.set_num(3);

    ASSERT_TRUE(tm.Validate(id1));
    EXPECT_TRUE(tm[id1].first.is_complete());
    EXPECT_EQ(tm[id1].first.title(), "Task 1");
    EXPECT_EQ(std::nullopt, tm[id1].second.parent());
    auto ch = tm[id1].second.children();
    ASSERT_EQ(1, ch.size());
    auto it = std::find(ch.cbegin(), ch.cend(), id2);
    EXPECT_NE(it, ch.cend());

    ASSERT_TRUE(tm.Validate(id2));
    EXPECT_TRUE(tm[id2].first.is_complete());
    EXPECT_EQ(tm[id2].first.title(), "Subtask 2");
    EXPECT_EQ(id1, *tm[id2].second.parent());
    ch = tm[id2].second.children();
    EXPECT_EQ(1, ch.size());
    it = std::find(ch.cbegin(), ch.cend(), id3);
    EXPECT_NE(it, ch.cend());

    ASSERT_TRUE(tm.Validate(id3));
    EXPECT_TRUE(tm[id3].first.is_complete());
    EXPECT_EQ(tm[id3].first.title(), "Subtask 3");
    EXPECT_EQ(0, tm[id3].second.children().size());
    EXPECT_EQ(id2, *tm[id3].second.parent());
}
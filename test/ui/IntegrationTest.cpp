//
// Created by Anton O. on 12/6/21.
//

#include "../../src/ui/Machine.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class IntegrationTest : public ::testing::Test
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

TEST_F(IntegrationTest, shouldCreateThreeTasksCompleteDelete)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
              std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(15)
            .WillOnce(Return("add"))
            .WillOnce(Return("test 1"))
            .WillOnce(Return("1"))
            .WillOnce(Return("21/12"))
            .WillOnce(Return("add"))
            .WillOnce(Return("test 2"))
            .WillOnce(Return("2"))
            .WillOnce(Return("22/12"))
            .WillOnce(Return("add"))
            .WillOnce(Return("test 3"))
            .WillOnce(Return("3"))
            .WillOnce(Return("23/12"))
            .WillOnce(Return("complete 1"))
            .WillOnce(Return("delete 3"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    ASSERT_EQ(2, tm.size());

    ProtoTask::TaskID id;
    id.set_num(1);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_TRUE(tm[id].first.is_complete());

    id.set_num(2);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_FALSE(tm[id].first.is_complete());

    id.set_num(3);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(IntegrationTest, shouldCreateTaskWithSubtasksCompleteAll)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(AtLeast(1))
            .WillOnce(Return("add"))
            .WillOnce(Return("test"))
            .WillOnce(Return("1"))
            .WillOnce(Return("21/12"))
            .WillOnce(Return("subtask 1"))
            .WillOnce(Return("sub"))
            .WillOnce(Return("2"))
            .WillOnce(Return("22/12"))
            .WillOnce(Return("subtask 2"))
            .WillOnce(Return("subsub"))
            .WillOnce(Return("3"))
            .WillOnce(Return("23/12"))
            .WillOnce(Return("complete 1"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    ASSERT_EQ(3, tm.size());

    // check completeness
    ProtoTask::TaskID id;
    id.set_num(1);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_TRUE(tm[id].first.is_complete());

    ProtoTask::TaskID id2;
    id2.set_num(2);
    ASSERT_TRUE(tm.Validate(id2));
    EXPECT_TRUE(tm[id2].first.is_complete());

    ProtoTask::TaskID id3;
    id3.set_num(3);
    ASSERT_TRUE(tm.Validate(id3));
    EXPECT_TRUE(tm[id3].first.is_complete());

    // check parents
    EXPECT_EQ(tm[id].second.parent(), std::nullopt);
    EXPECT_EQ(tm[id2].second.parent(), id);
    EXPECT_EQ(tm[id3].second.parent(), id2);

    // check children
    auto ch1 = tm[id].second.children();
    ASSERT_EQ(1, ch1.size());
    EXPECT_EQ(ch1[0], id2);

    auto ch2 = tm[id2].second.children();
    ASSERT_EQ(1, ch2.size());
    EXPECT_EQ(ch2[0], id3);

    auto ch3 = tm[id3].second.children();
    ASSERT_EQ(0, ch3.size());

}

TEST_F(IntegrationTest, shouldCreateTaskWithSubtasksLabelTwo)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(AtLeast(1))
            .WillOnce(Return("add"))
            .WillOnce(Return("task 1"))
            .WillOnce(Return("1"))
            .WillOnce(Return("21/12"))
            .WillOnce(Return("subtask 1"))
            .WillOnce(Return("sub"))
            .WillOnce(Return("2"))
            .WillOnce(Return("22/12"))
            .WillOnce(Return("add"))
            .WillOnce(Return("task 2"))
            .WillOnce(Return("3"))
            .WillOnce(Return("23/12"))
            .WillOnce(Return("label 3"))
            .WillOnce(Return("l3"))
            .WillOnce(Return("label 2"))
            .WillOnce(Return("l2"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    ASSERT_EQ(3, tm.size());

    // check labels
    ProtoTask::TaskID id;
    id.set_num(1);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_EQ("", tm[id].first.label());

    ProtoTask::TaskID id2;
    id2.set_num(2);
    ASSERT_TRUE(tm.Validate(id2));
    EXPECT_EQ("l2", tm[id2].first.label());

    ProtoTask::TaskID id3;
    id3.set_num(3);
    ASSERT_TRUE(tm.Validate(id3));
    EXPECT_EQ("l3", tm[id3].first.label());

    // check parents
    EXPECT_EQ(tm[id].second.parent(), std::nullopt);
    EXPECT_EQ(tm[id2].second.parent(), id);
    EXPECT_EQ(tm[id3].second.parent(), std::nullopt);

    // check children
    auto ch1 = tm[id].second.children();
    ASSERT_EQ(1, ch1.size());
    EXPECT_EQ(ch1[0], id2);

    auto ch2 = tm[id2].second.children();
    ASSERT_EQ(0, ch2.size());

    auto ch3 = tm[id3].second.children();
    ASSERT_EQ(0, ch3.size());

    EXPECT_EQ(4, tm.gen()->state());
}

TEST_F(IntegrationTest, shouldCreateThreeTasksDeleteTreeWithConfirm)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(AtLeast(1))
            .WillOnce(Return("add"))
            .WillOnce(Return("test 1"))
            .WillOnce(Return("1"))
            .WillOnce(Return("21/12"))
            .WillOnce(Return("add"))
            .WillOnce(Return("test 2"))
            .WillOnce(Return("2"))
            .WillOnce(Return("22/12"))
            .WillOnce(Return("subtask 1"))
            .WillOnce(Return("test 3"))
            .WillOnce(Return("3"))
            .WillOnce(Return("23/12"))
            .WillOnce(Return("delete 1"))
            .WillOnce(Return("N"))
            .WillOnce(Return("delete 1"))
            .WillOnce(Return("Y"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    ASSERT_EQ(1, tm.size());

    ProtoTask::TaskID id;
    id.set_num(1);
    ASSERT_FALSE(tm.Validate(id));
    id.set_num(2);
    ASSERT_TRUE(tm.Validate(id));
    id.set_num(3);
    EXPECT_FALSE(tm.Validate(id));
}

TEST_F(IntegrationTest, shouldCreateNothingWithBadInput)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(AtLeast(1))
            .WillOnce(Return("bad"))
            .WillOnce(Return("so bad"))
            .WillOnce(Return("so, so bad"))
            .WillOnce(Return(""))
            .WillOnce(Return("????"))
            .WillOnce(Return("help"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(5);

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    ASSERT_EQ(0, tm.size());
}

TEST_F(IntegrationTest, shouldCreateTaskWithSubtasksAndShowByID)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(AtLeast(1))
            .WillOnce(Return("add"))
            .WillOnce(Return("test"))
            .WillOnce(Return("1"))
            .WillOnce(Return("21/12"))
            .WillOnce(Return("subtask 1"))
            .WillOnce(Return("sub"))
            .WillOnce(Return("2"))
            .WillOnce(Return("22/12"))
            .WillOnce(Return("subtask 2"))
            .WillOnce(Return("subsub"))
            .WillOnce(Return("3"))
            .WillOnce(Return("23/12"))
            .WillOnce(Return("show 1"))
            .WillOnce(Return("show 2"))
            .WillOnce(Return("show 3"))
            .WillOnce(Return("show 4"))
            .WillOnce(Return("quit"));

    std::vector<std::string> out(19);
    int i = 0;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(19)
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]))
            .WillOnce(SaveArg<0>(&out[i++]));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    ASSERT_EQ(3, tm.size());

    EXPECT_EQ(out[6], "1 – test, Priority: Low, Due: Tue Dec 21 00:00:00 2021");
    EXPECT_EQ(out[8], "    2 – sub, Priority: Medium, Due: Wed Dec 22 00:00:00 2021");
    EXPECT_EQ(out[10], "        3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021");
    EXPECT_EQ(out[12], "2 – sub, Priority: Medium, Due: Wed Dec 22 00:00:00 2021");
    EXPECT_EQ(out[14], "    3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021");
    EXPECT_EQ(out[16], "3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021");
    EXPECT_EQ(out[18], "Invalid ID. Try again.\n");

}

TEST_F(IntegrationTest, shouldCreateThreeTasksInAHeirarcySaveAndLoad)
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
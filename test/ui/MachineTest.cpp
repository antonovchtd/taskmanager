//
// Created by Anton O. on 12/6/21.
//

#include "../../src/ui/Machine.h"
#include "../../src/ui/Step.h"
#include "../../src/controller/Action.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

class MachineTest : public ::testing::Test
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

TEST_F(MachineTest, shouldCreateThreeTasksCompleteDelete)
{
    Factory f(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f.reader()), read(_))
            .Times(AtLeast(1))
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

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f.printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = m.model();
    ASSERT_EQ(2, tm.size());

    ASSERT_TRUE(tm.Validate(TaskID(1)));
    EXPECT_TRUE(tm[TaskID(1)].first.isComplete());

    ASSERT_TRUE(tm.Validate(TaskID(2)));
    EXPECT_FALSE(tm[TaskID(2)].first.isComplete());

    EXPECT_FALSE(tm.Validate(TaskID(3)));
}

TEST_F(MachineTest, shouldCreateTaskWithSubtasksCompleteTwo)
{
    Factory f(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f.reader()), read(_))
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
            .WillOnce(Return("complete 3"))
            .WillOnce(Return("complete 1"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f.printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = m.model();
    ASSERT_EQ(3, tm.size());

    // check completeness
    ASSERT_TRUE(tm.Validate(TaskID(1)));
    EXPECT_TRUE(tm[TaskID(1)].first.isComplete());

    ASSERT_TRUE(tm.Validate(TaskID(2)));
    EXPECT_FALSE(tm[TaskID(2)].first.isComplete());

    ASSERT_TRUE(tm.Validate(TaskID(3)));
    EXPECT_TRUE(tm[TaskID(3)].first.isComplete());

    // check parents
    EXPECT_EQ(tm[TaskID(1)].second.parent(), TaskID::invalidID());
    EXPECT_EQ(tm[TaskID(2)].second.parent(), TaskID(1));
    EXPECT_EQ(tm[TaskID(3)].second.parent(), TaskID(2));

    // check children
    auto ch1 = tm[TaskID(1)].second.children();
    ASSERT_EQ(1, ch1.size());
    EXPECT_EQ(ch1[0], TaskID(2));

    auto ch2 = tm[TaskID(2)].second.children();
    ASSERT_EQ(1, ch2.size());
    EXPECT_EQ(ch2[0], TaskID(3));

    auto ch3 = tm[TaskID(3)].second.children();
    ASSERT_EQ(0, ch3.size());

}

TEST_F(MachineTest, shouldCreateTaskWithSubtasksLabelTwo)
{
    std::istringstream is{
            std::string("add\ntask 1\n1\n21/12\nsubtask 1\nsub\n2\n22/12\n") +
            std::string("add\ntask 2\n3\n23/12\nlabel 3\nl3\nlabel 2\nl2\nquit")};

    Factory f(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f.reader()), read(_))
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

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f.printer()), print(_))
            .Times(AtLeast(1));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = m.model();
    ASSERT_EQ(3, tm.size());

    // check labels
    ASSERT_TRUE(tm.Validate(TaskID(1)));
    EXPECT_EQ("", tm[TaskID(1)].second.label());

    ASSERT_TRUE(tm.Validate(TaskID(2)));
    EXPECT_EQ("l2", tm[TaskID(2)].second.label());

    ASSERT_TRUE(tm.Validate(TaskID(3)));
    EXPECT_EQ("l3", tm[TaskID(3)].second.label());

    // check parents
    EXPECT_EQ(tm[TaskID(1)].second.parent(), TaskID::invalidID());
    EXPECT_EQ(tm[TaskID(2)].second.parent(), TaskID(1));
    EXPECT_EQ(tm[TaskID(3)].second.parent(), TaskID::invalidID());

    // check children
    auto ch1 = tm[TaskID(1)].second.children();
    ASSERT_EQ(1, ch1.size());
    EXPECT_EQ(ch1[0], TaskID(2));

    auto ch2 = tm[TaskID(2)].second.children();
    ASSERT_EQ(0, ch2.size());

    auto ch3 = tm[TaskID(3)].second.children();
    ASSERT_EQ(0, ch3.size());

}
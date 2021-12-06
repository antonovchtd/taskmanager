//
// Created by Anton O. on 12/6/21.
//

#include "../../src/ui/Machine.h"

#include <gtest/gtest.h>

class MachineTest : public ::testing::Test
{

};

TEST_F(MachineTest, shouldCreateThreeTasksCompleteDelete)
{
    std::istringstream is{
    std::string("add\ntest 1\n1\n21/12\nadd\ntest 2\n2\n22/12\n") +
    std::string("add\ntest 3\n3\n23/12\ncomplete 1\ndelete 3\nquit")};
    std::ostringstream os{};
    Context c;
    c.setIStream(is);
    c.setOStream(os);
    Machine m(c, Factory::State::HOME);
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
    std::istringstream is{
    std::string("add\ntest\n1\n21/12\nsubtask 1\nsub\n2\n22/12\n") +
    std::string("subtask 2\nsubsub\n3\n23/12\ncomplete 3\ncomplete 1\nquit")};
    std::ostringstream os{};
    Context c;
    c.setIStream(is);
    c.setOStream(os);
    Machine m(c, Factory::State::HOME);
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
    std::ostringstream os{};
    Context c;
    c.setIStream(is);
    c.setOStream(os);
    Machine m(c, Factory::State::HOME);
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
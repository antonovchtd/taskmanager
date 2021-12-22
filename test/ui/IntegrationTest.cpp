//
// Created by Anton O. on 12/6/21.
//

#include <numeric>

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

class MockReaderToVector : public AbstractReader {
public:
    MockReaderToVector() { }
    explicit MockReaderToVector(const std::vector<std::string> &in) : in_{in} { }
    std::string read(const std::string &prompt) override {
        prompts_.push_back(prompt);
        auto it = in_.begin();
        auto res = *it;
        in_.erase(it);
        return res;
    }
    std::vector<std::string> prompts() const { return prompts_; }

private:
    std::vector<std::string> prompts_;
    std::vector<std::string> in_;
};

class MockPrinterToVector : public AbstractPrinter {
public:
    void print(const std::string &message) override {
        messages_.push_back(message);
    }
    std::vector<std::string> messages() const { return messages_; }

private:
    std::vector<std::string> messages_;
};

TEST_F(IntegrationTest, shouldCreateThreeTasksCompleteOneDeleteOne)
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
    id.set_value(1);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_TRUE(tm[id].first.is_complete());

    id.set_value(2);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_FALSE(tm[id].first.is_complete());

    id.set_value(3);
    EXPECT_FALSE(tm.Validate(id));

    EXPECT_EQ(4, tm.gen()->state());
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
    id.set_value(1);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_TRUE(tm[id].first.is_complete());

    ProtoTask::TaskID id2;
    id2.set_value(2);
    ASSERT_TRUE(tm.Validate(id2));
    EXPECT_TRUE(tm[id2].first.is_complete());

    ProtoTask::TaskID id3;
    id3.set_value(3);
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

    EXPECT_EQ(4, tm.gen()->state());
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
    id.set_value(1);
    ASSERT_TRUE(tm.Validate(id));
    EXPECT_EQ("", tm[id].first.label());

    ProtoTask::TaskID id2;
    id2.set_value(2);
    ASSERT_TRUE(tm.Validate(id2));
    EXPECT_EQ("l2", tm[id2].first.label());

    ProtoTask::TaskID id3;
    id3.set_value(3);
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
    std::vector<std::string> scenario = {"add", "test 1", "1", "21/12", "add", "test 2",
                                         "2", "22/12", "subtask 1", "test 3", "3", "23/12",
                                         "delete 1", "N", "delete 1", "Y", "quit"};

    std::vector<std::string> prompts_expected = {" > ", "    Title > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 " > ", "    Title > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 " > ", "    Title > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 " > ", "Task 1 has 1 subtask(s). Confirm to delete all. [Y]/N > ",
                                                 " > ", "Task 1 has 1 subtask(s). Confirm to delete all. [Y]/N > ",
                                                 " > "};
    std::string expected_output = "[Add Task]\nAdded Task (ID: 1)\n[Add Task]\nAdded Task (ID: 2)\n[Add Subtask]\nAdded Subtask (ID: 3)\nDeleted Task (ID: 1)\n";
    MockReaderToVector mr{scenario};
    MockPrinterToVector mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    auto out = mp.messages();
    auto prompts = mr.prompts();
    ASSERT_EQ(1, tm.size());

    ProtoTask::TaskID id;
    id.set_value(1);
    EXPECT_FALSE(tm.Validate(id));
    id.set_value(2);
    EXPECT_TRUE(tm.Validate(id));
    id.set_value(3);
    EXPECT_FALSE(tm.Validate(id));

    for (int i = 0; i <  prompts.size(); ++i) {
        EXPECT_EQ(prompts[i], prompts_expected[i]);
    }

    std::string accumulated_output = std::accumulate(out.cbegin(), out.cend(), std::string());
    EXPECT_EQ(accumulated_output, expected_output);
}

TEST_F(IntegrationTest, shouldCreateTaskWithBadInputs)
{
    std::vector<std::string> scenario = {"", "???", "add", "", "test title", "42", "-1",
                                         "", "", "21,12", "21/12/2021", "show", "quit"};

    std::vector<std::string> prompts_expected = {" > ", " > ", " > ",
                                                 "    Title > ", "    Title > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 " > ",  " > "};
    std::string expected_output = "Wrong command. Try again. Type `help` for help.\n"
                                  "[Add Task]\n"
                                  "    Title cannot be empty!\n"
                                  "    Wrong priority option. Try again.\n"
                                  "    Wrong priority option. Try again.\n"
                                  "    Wrong date format. Try again.\n"
                                  "    Wrong date format. Try again.\n"
                                  "Added Task (ID: 1)\n"
                                  "1 – test title, Priority: None, Due: Tue Dec 21 00:00:00 2021 [overdue]\n";

    MockReaderToVector mr{scenario};
    MockPrinterToVector mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();
    auto out = mp.messages();
    auto prompts = mr.prompts();
    ASSERT_EQ(1, tm.size());

    ProtoTask::TaskID id;
    id.set_value(1);
    EXPECT_TRUE(tm.Validate(id));

    for (int i = 0; i <  prompts.size(); ++i) {
        EXPECT_EQ(prompts[i], prompts_expected[i]);
    }

    std::string accumulated_output = std::accumulate(out.cbegin(), out.cend(), std::string());
    EXPECT_EQ(accumulated_output, expected_output);
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
    std::vector<std::string> scenario {"add", "test", "1", "21/12", "subtask 1", "sub", "2",
                                       "22/12", "subtask 2", "subsub", "3", "23/12", "show 1",
                                       "show 2", "show 3", "show 4", "quit"};
    MockReaderToVector mr{scenario};
    MockPrinterToVector mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    Machine m(f, Factory::State::HOME);
    m.run();
    TaskManager tm = *m.model();

    auto out = mp.messages();
    ASSERT_EQ(3, tm.size());

    EXPECT_EQ(out[9], "1 – test, Priority: Low, Due: Tue Dec 21 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[11], "    2 – sub, Priority: Medium, Due: Wed Dec 22 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[13], "        3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021");
    EXPECT_EQ(out[15], "2 – sub, Priority: Medium, Due: Wed Dec 22 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[17], "    3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021");
    EXPECT_EQ(out[19], "3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021");
    EXPECT_EQ(out[21], "ID 4 was not found.\n");

}

TEST_F(IntegrationTest, shouldCreateThreeTasksInAHeirarcySaveAndLoad)
{
    std::vector<std::string> scenario = {"add", "Task 1", "1", "21/12", "subtask 1", "Subtask 2",
                                         "2", "22/12", "subtask 2", "Subtask 3", "3", "23/12",
                                         "complete 1", "save", "quit", "load", "quit"};
    MockReaderToVector mr{scenario};
    MockPrinterToVector mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    Machine m(f, Factory::State::HOME);
    m.run();
    Machine m2(f, Factory::State::HOME);
    m2.run();
    TaskManager tm = *m2.model();
    ASSERT_EQ(3, tm.size());

    ProtoTask::TaskID id1, id2, id3;
    id1.set_value(1);
    id2.set_value(2);
    id3.set_value(3);

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
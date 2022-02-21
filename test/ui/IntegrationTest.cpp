//
// Created by Anton O. on 12/6/21.
//

#include <numeric>

#include "ui/Machine.h"
#include "utilities/TaskUtils.h"

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
    auto tm = std::make_shared<TaskManager>();
    auto mr = std::make_shared<MockReader>();
    auto mp = std::make_shared<MockPrinter>();
    auto f = Factory::create(std::shared_ptr<AbstractReader>(mr),
              std::shared_ptr<AbstractPrinter>(mp));
    EXPECT_CALL(*mr, read)
            .Times(17)
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
            .WillOnce(Return("?"))
            .WillOnce(Return("Y"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*mp, print)
            .Times(AtLeast(1));

    Machine m(tm, f, Factory::State::HOME);
    m.run();
    ASSERT_EQ(2, tm->size());

    Core::TaskID id;
    id.set_value(1);
    ASSERT_TRUE(ToBool(tm->IsPresent(id)));
    EXPECT_EQ(tm->getTasks()[0].id(), id);
    EXPECT_TRUE(tm->getTasks()[0].data().is_complete());

    id.set_value(2);
    ASSERT_TRUE(ToBool(tm->IsPresent(id)));
    EXPECT_EQ(tm->getTasks()[1].id(), id);
    EXPECT_FALSE(tm->getTasks()[1].data().is_complete());

    id.set_value(3);
    EXPECT_FALSE(ToBool(tm->IsPresent(id)));

    EXPECT_EQ(4, tm->gen()->state());
}

TEST_F(IntegrationTest, shouldCreateTaskWithSubtasksCompleteAll)
{
    auto tm = std::make_shared<TaskManager>();
    auto mr = std::make_shared<MockReader>();
    auto mp = std::make_shared<MockPrinter>();
    auto f = Factory::create(std::shared_ptr<AbstractReader>(mr),
                             std::shared_ptr<AbstractPrinter>(mp));
    EXPECT_CALL(*mr, read(_))
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

    EXPECT_CALL(*mp, print(_))
            .Times(AtLeast(1));

    Machine m(tm, f, Factory::State::HOME);
    m.run();
    ASSERT_EQ(3, tm->size());

    // check completeness
    Core::TaskID id;
    id.set_value(1);
    ASSERT_TRUE(ToBool(tm->IsPresent(id)));
    EXPECT_EQ(tm->getTasks()[0].id(), id);
    EXPECT_TRUE(tm->getTasks()[0].data().is_complete());

    Core::TaskID id2;
    id2.set_value(2);
    ASSERT_TRUE(ToBool(tm->IsPresent(id2)));
    EXPECT_EQ(tm->getTasks()[1].id(), id2);
    EXPECT_TRUE(tm->getTasks()[1].data().is_complete());

    Core::TaskID id3;
    id3.set_value(3);
    ASSERT_TRUE(ToBool(tm->IsPresent(id3)));
    EXPECT_EQ(tm->getTasks()[2].id(), id3);
    EXPECT_TRUE(tm->getTasks()[2].data().is_complete());

    // check parents
    EXPECT_FALSE(tm->getTasks()[0].has_parent());
    EXPECT_EQ(tm->getTasks()[1].parent(), id);
    EXPECT_EQ(tm->getTasks()[2].parent(), id2);

    EXPECT_EQ(4, tm->gen()->state());
}

TEST_F(IntegrationTest, shouldCreateTaskWithSubtasksLabelTwo)
{
    auto tm = std::make_shared<TaskManager>();
    auto mr = std::make_shared<MockReader>();
    auto mp = std::make_shared<MockPrinter>();
    auto f = Factory::create(std::shared_ptr<AbstractReader>(mr),
                             std::shared_ptr<AbstractPrinter>(mp));
    EXPECT_CALL(*mr, read)
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
            .WillOnce(Return(""))
            .WillOnce(Return("l3"))
            .WillOnce(Return("label 2"))
            .WillOnce(Return("l2"))
            .WillOnce(Return("quit"));

    EXPECT_CALL(*mp, print)
            .Times(AtLeast(1));

    Machine m(tm, f, Factory::State::HOME);
    m.run();
    ASSERT_EQ(3, tm->size());

    auto tasks = tm->getTasks();
    // check labels
    Core::TaskID id;
    id.set_value(1);
    ASSERT_TRUE(ToBool(tm->IsPresent(id)));
    EXPECT_TRUE(tasks[0].data().labels().empty());

    Core::TaskID id2;
    id2.set_value(2);
    ASSERT_TRUE(ToBool(tm->IsPresent(id2)));
    EXPECT_EQ(1, tasks[1].data().labels().size());
    EXPECT_EQ("l2", tasks[1].data().labels()[0]);

    Core::TaskID id3;
    id3.set_value(3);
    ASSERT_TRUE(ToBool(tm->IsPresent(id3)));
    EXPECT_EQ(1, tasks[2].data().labels().size());
    EXPECT_EQ("l3", tasks[2].data().labels()[0]);

    // check parents
    EXPECT_FALSE(tm->getTasks()[0].has_parent());
    EXPECT_EQ(tm->getTasks()[1].parent(), id);
    EXPECT_FALSE(tm->getTasks()[2].has_parent());

    EXPECT_EQ(4, tm->gen()->state());
}

TEST_F(IntegrationTest, shouldCreateThreeTasksDeleteTwoWithConfirm)
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
                                                 " > ", "Delete task with all its subtasks? Y/[N] > ",
                                                 " > ", "Delete task with all its subtasks? Y/[N] > ",
                                                 " > "};
    std::string expected_output = "[Add Task]\nAdded Task (ID: 1)\n[Add Task]\nAdded Task (ID: 2)\n[Add Subtask]\nAdded Subtask (ID: 3)\nDeleted Task (ID: 1)\n";
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();
    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    auto mr = *std::dynamic_pointer_cast<MockReaderToVector>(f->reader());
    auto prompts = mr.prompts();
    ASSERT_EQ(1, tm->size());

    Core::TaskID id;
    id.set_value(1);
    ASSERT_FALSE(ToBool(tm->IsPresent(id)));
    id.set_value(2);
    ASSERT_TRUE(ToBool(tm->IsPresent(id)));
    id.set_value(3);
    ASSERT_FALSE(ToBool(tm->IsPresent(id)));

    for (int i = 0; i <  prompts.size(); ++i) {
        EXPECT_EQ(prompts[i], prompts_expected[i]);
    }

    std::string accumulated_output = std::accumulate(out.cbegin(), out.cend(), std::string());
    EXPECT_EQ(accumulated_output, expected_output);
}

TEST_F(IntegrationTest, shouldCreateTaskWithBadInputs)
{
    std::vector<std::string> scenario = {"", "???", "add", "", "test title", "42", "low",
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

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();
    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    auto mr = *std::dynamic_pointer_cast<MockReaderToVector>(f->reader());
    auto prompts = mr.prompts();
    ASSERT_EQ(1, tm->size());

    Core::TaskID id;
    id.set_value(1);
    EXPECT_TRUE(ToBool(tm->IsPresent(id)));

    for (int i = 0; i <  prompts.size(); ++i) {
        EXPECT_EQ(prompts[i], prompts_expected[i]);
    }

    std::string accumulated_output = std::accumulate(out.cbegin(), out.cend(), std::string());
    EXPECT_EQ(accumulated_output, expected_output);
}

TEST_F(IntegrationTest, shouldCreateNothingWithBadInput)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
                             std::shared_ptr<AbstractPrinter>(new MockPrinter));
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

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();
    ASSERT_EQ(0, tm->size());
}

TEST_F(IntegrationTest, shouldCreateTaskWithSubtasksAndShowByID)
{
    std::vector<std::string> scenario {"add", "test", "1", "21/12/21", "subtask 1", "sub", "2",
                                       "22/12/21", "subtask 2", "subsub", "3", "23/12/21", "show 1",
                                       "show 2", "show 3", "show 4", "quit"};
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    MockPrinterToVector mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(3, tm->size());

    EXPECT_EQ(out[9], "1 – test, Priority: Low, Due: Tue Dec 21 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[11], "    2 – sub, Priority: Medium, Due: Wed Dec 22 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[13], "        3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[15], "2 – sub, Priority: Medium, Due: Wed Dec 22 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[17], "    3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[19], "3 – subsub, Priority: High, Due: Thu Dec 23 00:00:00 2021 [overdue]");
    EXPECT_EQ(out[21], "ID was not found.\n");

}

TEST_F(IntegrationTest, shouldCreateThreeTasksInAHeirarcySaveAndLoad)
{
    std::vector<std::string> scenario = {"add", "Task 1", "1", "21/12", "subtask 1", "Subtask 2",
                                         "2", "22/12", "subtask 2", "Subtask 3", "3", "23/12",
                                         "complete 1", "save test.bin", "quit", "load test.bin", "quit"};
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();
    tm.reset();
    tm = std::make_shared<TaskManager>();
    Machine m2(tm, f, Factory::State::HOME);
    m2.run();
    ASSERT_EQ(3, tm->size());

    Core::TaskID id1, id2, id3;
    id1.set_value(1);
    id2.set_value(2);
    id3.set_value(3);

    ASSERT_TRUE(ToBool(tm->IsPresent(id1)));
    EXPECT_TRUE(tm->getTasks()[0].data().is_complete());
    EXPECT_EQ(tm->getTasks()[0].data().title(), "Task 1");
    EXPECT_FALSE(tm->getTasks()[0].has_parent());

    ASSERT_TRUE(ToBool(tm->IsPresent(id2)));
    EXPECT_TRUE(tm->getTasks()[1].data().is_complete());
    EXPECT_EQ(tm->getTasks()[1].data().title(), "Subtask 2");
    EXPECT_EQ(id1, tm->getTasks()[1].parent());

    ASSERT_TRUE(ToBool(tm->IsPresent(id3)));
    EXPECT_TRUE(tm->getTasks()[2].data().is_complete());
    EXPECT_EQ(tm->getTasks()[2].data().title(), "Subtask 3");
    EXPECT_EQ(id2, tm->getTasks()[2].parent());
}

TEST_F(IntegrationTest, shouldEditTask)
{
    std::vector<std::string> scenario = {"add", "Test Task", "1", "21/12/21", "show", "edit",
                                         "edit 1", "edited", "2", "", "show", "quit"};

    std::vector<std::string> prompts_expected = {" > ", "    Title > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 " > ", " > ", " > ", "    Title [Test Task] > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) [Low] > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} [Tue Dec 21 00:00:00 2021] > ",
                                                 " > ", " > "};

    std::vector<std::string> expected_outputs = {"[Add Task]\n",
                                  "Added Task", " (ID: 1)\n",
                                  "1 – Test Task, Priority: Low, Due: Tue Dec 21 00:00:00 2021 [overdue]", "\n",
                                  "This function requires an ID argument.\n",
                                  "[Edit Task]\n",
                                  "Edited Task", " (ID: 1)\n",
                                  "1 – edited, Priority: Medium, Due: Tue Dec 21 00:00:00 2021 [overdue]", "\n"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(1, tm->size());

    auto mr = *std::dynamic_pointer_cast<MockReaderToVector>(f->reader());
    auto prompts = mr.prompts();
    ASSERT_EQ(prompts.size(), prompts_expected.size());
    for (int i = 0; i <  prompts.size(); ++i) {
        EXPECT_EQ(prompts[i], prompts_expected[i]);
    }

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(out.size(), expected_outputs.size());
    for (int i = 0; i <  out.size(); ++i) {
        EXPECT_EQ(out[i], expected_outputs[i]);
    }
}

TEST_F(IntegrationTest, shouldUncompleteTaskWithSubtasks)
{
    std::vector<std::string> scenario = {"add", "test", "1", "21/12/21",
                                         "subtask 1", "sub", "2", "22/12/21 15:10",
                                         "complete 1", "show", "uncomplete 1", "show", "quit"};

    std::vector<std::string> expected_outputs = {"[Add Task]\n",
                                                 "Added Task", " (ID: 1)\n",
                                                 "[Add Subtask]\n",
                                                 "Added Subtask", " (ID: 2)\n",
                                                 "Completed Task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Tue Dec 21 00:00:00 2021 [overdue] [completed]", "\n",
                                                 "    2 – sub, Priority: Medium, Due: Wed Dec 22 15:10:00 2021 [overdue] [completed]", "\n",
                                                 "Uncompleted Task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Tue Dec 21 00:00:00 2021 [overdue]", "\n",
                                                 "    2 – sub, Priority: Medium, Due: Wed Dec 22 15:10:00 2021 [overdue]", "\n"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(2, tm->size());

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(out.size(), expected_outputs.size());
    for (int i = 0; i <  out.size(); ++i) {
        EXPECT_EQ(out[i], expected_outputs[i]);
    }
}

TEST_F(IntegrationTest, shouldAddTasksInFutureDate)
{
    std::vector<std::string> scenario = {"add", "test 1", "1", "in 01:02:03",
                                         "add", "test 2", "2", "in 01:02",
                                         "show", "quit"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(2, tm->size());

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();

    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_mday++;
    timeinfo->tm_hour+=2;
    timeinfo->tm_min+=3;
    time_t date = mktime(timeinfo);
    EXPECT_EQ(out[6] + "\n", std::string("1 – test 1, Priority: Low, Due: ") + asctime(localtime(&date)));

    struct tm * timeinfo2 = localtime(&rawtime);
    timeinfo->tm_hour+=1;
    timeinfo->tm_min+=2;
    date = mktime(timeinfo);
    EXPECT_EQ(out[8] + "\n", std::string("2 – test 2, Priority: Medium, Due: ") + asctime(localtime(&date)));
}

TEST_F(IntegrationTest, shouldAddMultipleLabels)
{
    std::vector<std::string> scenario = {"add", "test", "1", "21/12/25",
                                         "label 1", "l1", "label 1", "l2", "label 1", "l3", "show",
                                         "quit"};

    std::vector<std::string> expected_outputs = {"[Add Task]\n",
                                                 "Added Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Sun Dec 21 00:00:00 2025, has 3 label(s)", "\n"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(1, tm->size());

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(out.size(), expected_outputs.size());
    for (int i = 0; i <  out.size(); ++i) {
        EXPECT_EQ(out[i], expected_outputs[i]);
    }
}


TEST_F(IntegrationTest, shouldClearSomeLabels)
{
    std::vector<std::string> scenario = {"add", "test", "1", "21/12/25",
                                         "label 1", "l1", "label 1", "l2", "label 1", "l3", "show",
                                         "unlabel 1", "", "l1", "unlabel 1", "l3", "show", "quit"};

    std::vector<std::string> expected_outputs = {"[Add Task]\n",
                                                 "Added Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Sun Dec 21 00:00:00 2025, has 3 label(s)", "\n",
                                                 "Removed label from task", " (ID: 1)\n",
                                                 "Removed label from task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Sun Dec 21 00:00:00 2025, has 1 label(s)", "\n"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(1, tm->size());

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(out.size(), expected_outputs.size());
    for (int i = 0; i <  out.size(); ++i) {
        EXPECT_EQ(out[i], expected_outputs[i]);
    }
}

TEST_F(IntegrationTest, shouldClearAllLabels)
{
    std::vector<std::string> scenario = {"add", "test", "1", "21/12/25",
                                         "label 1", "l1", "label 1", "l2", "label 1", "l3", "show",
                                         "UNLABEL 1", "show", "quit"};

    std::vector<std::string> expected_outputs = {"[Add Task]\n",
                                                 "Added Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Sun Dec 21 00:00:00 2025, has 3 label(s)", "\n",
                                                 "Removed all labels of the task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Sun Dec 21 00:00:00 2025", "\n"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(1, tm->size());

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(out.size(), expected_outputs.size());
    for (int i = 0; i <  out.size(); ++i) {
        EXPECT_EQ(out[i], expected_outputs[i]);
    }
}

TEST_F(IntegrationTest, shouldAddAndShowMultipleLabels)
{
    std::vector<std::string> scenario = {"add", "test", "1", "21/12/25",
                                         "label 1", "l1", "label 1", "l2", "label 1", "l3", "show",
                                         "labels 1", "quit"};

    std::vector<std::string> expected_outputs = {"[Add Task]\n",
                                                 "Added Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "Added label to Task", " (ID: 1)\n",
                                                 "1 – test, Priority: Low, Due: Sun Dec 21 00:00:00 2025, has 3 label(s)", "\n",
                                                 "l1, l2, l3\n"};

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector{scenario}),
                             std::shared_ptr<AbstractPrinter>(new MockPrinterToVector));

    auto tm = std::make_shared<TaskManager>();
    Machine m(tm, f, Factory::State::HOME);
    m.run();

    ASSERT_EQ(1, tm->size());

    auto mp = *std::dynamic_pointer_cast<MockPrinterToVector>(f->printer());
    auto out = mp.messages();
    ASSERT_EQ(out.size(), expected_outputs.size());
    for (int i = 0; i <  out.size(); ++i) {
        EXPECT_EQ(out[i], expected_outputs[i]);
    }
}
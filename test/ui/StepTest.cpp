//
// Created by Anton O. on 12/4/21.
//

#include "ui/Step.h"
#include "ui/Context.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::_;
using testing::SaveArg;

class StepTest : public ::testing::Test
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

class MockController : public ControllerInterface {
public:
    MOCK_METHOD(ActionResult, ValidateID, (Context &), (override));
    MOCK_METHOD(ActionResult, ValidateNoArg, (Context &), (override));
    MOCK_METHOD(ActionResult, ValidateLabelOrID, (Context &), (override));
    MOCK_METHOD(ActionResult, ValidateAlpha, (Context &), (override));
    MOCK_METHOD(ActionResult, AddTask, (Context &), (override));
    MOCK_METHOD(ActionResult, EditTask, (Context &), (override));
    MOCK_METHOD(ActionResult, AddSubtask, (Context &), (override));
    MOCK_METHOD(ActionResult, ShowTasks, (Context &), (override));
    MOCK_METHOD(ActionResult, CompleteTask, (Context &), (override));
    MOCK_METHOD(ActionResult, UncompleteTask, (Context &), (override));
    MOCK_METHOD(ActionResult, DeleteTask, (Context &), (override));
    MOCK_METHOD(ActionResult, ReadTaskWithChildren, (Context &), (override));
    MOCK_METHOD(ActionResult, LabelTask, (Context &), (override));
    MOCK_METHOD(ActionResult, SaveTasks, (Context &), (override));
    MOCK_METHOD(ActionResult, LoadTasks, (Context &), (override));
    MOCK_METHOD(void, setData, (const ControllerInterface::Data &), (override));
    MOCK_METHOD(std::shared_ptr<TaskManagerInterface>, model, (), (const, override));
    MOCK_METHOD(ControllerInterface::Data, data, (), (const, override));
};

std::vector<std::string> homeStepExecuteCall(const std::string &command, Factory::State expected_step) {
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  time(nullptr), "label", false));
    auto con = std::make_shared<Controller>(tm);
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
                                               std::shared_ptr<AbstractPrinter>(new MockPrinterToVector),
                                               con);
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
        .Times(1)
        .WillOnce(Return(command));

    auto step = f->lazyInitStep(Factory::State::HOME);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(expected_step));
    std::istringstream is{command};
    std::string function, argument;
    is >> function >> argument;
    EXPECT_EQ(f->controller()->data().arg, argument);
    return std::dynamic_pointer_cast<MockPrinterToVector>(f->printer())->messages();
}

TEST_F(StepTest, executeHomeStepWithAddCommand)
{
    auto messages = homeStepExecuteCall("add", Factory::State::ADD);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithAddCommandWithArgument)
{
    auto messages = homeStepExecuteCall("add 1", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function takes no argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithHelpCommand)
{
    auto messages = homeStepExecuteCall("help", Factory::State::HELP);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithHelpCommandWithArgument)
{
    auto messages = homeStepExecuteCall("help add", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function takes no argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithQuitCommand)
{
    auto messages = homeStepExecuteCall("quit", Factory::State::QUIT);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithQuitCommandWithArgument)
{
    auto messages = homeStepExecuteCall("quit all", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function takes no argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithShowCommand)
{
    auto messages = homeStepExecuteCall("show", Factory::State::SHOW);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithShowCommandWithValidID)
{
    auto messages = homeStepExecuteCall("show 1", Factory::State::SHOW);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithShowCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("show 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithShowCommandWithValidLabel)
{
    auto messages = homeStepExecuteCall("show label", Factory::State::SHOW);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithShowCommandWithInalidLabel)
{
    auto messages = homeStepExecuteCall("show not_label", Factory::State::SHOW);
    EXPECT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithEditCommand)
{
    auto messages = homeStepExecuteCall("edit", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires an argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithEditCommandWithValidID)
{
    auto messages = homeStepExecuteCall("edit 1", Factory::State::EDIT);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithEditCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("edit 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithSubtaskCommand)
{
    auto messages = homeStepExecuteCall("subtask", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires an argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithSubtaskCommandWithValidID)
{
    auto messages = homeStepExecuteCall("subtask 1", Factory::State::SUBTASK);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithSubtaskCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("subtask 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithCompleteCommand)
{
    auto messages = homeStepExecuteCall("complete", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires an argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithComlpeteCommandWithValidID)
{
    auto messages = homeStepExecuteCall("complete 1", Factory::State::COMPLETE);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithCompleteCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("complete 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithUncompleteCommand)
{
    auto messages = homeStepExecuteCall("uncomplete", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires an argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithUncompleteCommandWithValidID)
{
    auto messages = homeStepExecuteCall("uncomplete 1", Factory::State::UNCOMPLETE);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithUncompleteCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("uncomplete 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithDeleteCommand)
{
    auto messages = homeStepExecuteCall("delete", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires an argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithDeleteCommandWithValidID)
{
    auto messages = homeStepExecuteCall("delete 1", Factory::State::CONFIRMDELETE);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithDeleteCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("delete 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithLabelCommand)
{
    auto messages = homeStepExecuteCall("label", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires an argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithLabelCommandWithValidID)
{
    auto messages = homeStepExecuteCall("label 1", Factory::State::LABEL);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithLabelCommandWithInvalidID)
{
    auto messages = homeStepExecuteCall("label 2", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("ID 2 was not found.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithLoadCommand)
{
    auto messages = homeStepExecuteCall("load", Factory::State::LOAD);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithLoadCommandWithID)
{
    auto messages = homeStepExecuteCall("load 1", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("This function requires a word, not an ID argument.\n", messages[0]);
}

TEST_F(StepTest, executeHomeStepWithLoadCommandWithFilename)
{
    auto messages = homeStepExecuteCall("load myfile.txt", Factory::State::LOAD);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithEmptyCommand)
{
    auto messages = homeStepExecuteCall("", Factory::State::HOME);
    ASSERT_EQ(0, messages.size());
}

TEST_F(StepTest, executeHomeStepWithBadCommand)
{
    auto messages = homeStepExecuteCall("bad", Factory::State::HOME);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("Wrong command. Try again. Type `help` for help.\n", messages[0]);
}

std::vector<std::string> executeWithWizardCall(Factory::State current_step, Factory::State expected_next_step) {
    std::vector<std::string> scenario = {"", "test", "10", "low", "", "31-12-22", "31/12/2022 15:00"};
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  time(nullptr), "label", false));
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>(scenario);
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    std::vector<std::string> expected_prompts = {"    Title > ", "    Title > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > "
    };

    auto step = f->lazyInitStep(current_step);
    Context c;
    ProtoTask::TaskID id;
    id.set_value(1);
    c.setID(id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(expected_next_step));

    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour = 15;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    timeinfo->tm_mon = 11;
    timeinfo->tm_mday = 31;
    timeinfo->tm_year = 122;
    ProtoTask::Task t = ProtoTask::createTask("test", ProtoTask::Task_Priority_NONE, mktime(timeinfo), "", false);
    EXPECT_EQ(t, c.task());

    auto prompts = mr->prompts();
    EXPECT_EQ(prompts.size(), expected_prompts.size());
    for (int i = 0; i < prompts.size(); ++i)
        EXPECT_EQ(prompts[i], expected_prompts[i]);

    auto messages = mp->messages();
    EXPECT_EQ(messages[1], "    Title cannot be empty!\n");
    EXPECT_EQ(messages[2], "    Wrong priority option. Try again.\n");
    EXPECT_EQ(messages[3], "    Wrong priority option. Try again.\n");
    EXPECT_EQ(messages[4], "    Wrong date format. Try again.\n");

    return messages;
}

TEST_F(StepTest, executeAddStep)
{
    auto messages = executeWithWizardCall(Factory::State::ADD, Factory::State::HOME);
    EXPECT_EQ(messages[0], "[Add Task]\n");
    EXPECT_EQ(messages[5], "Added Task");
    EXPECT_EQ(messages[6], " (ID: 2)\n");
}

TEST_F(StepTest, executeSubtaskStep)
{
    auto messages = executeWithWizardCall(Factory::State::SUBTASK, Factory::State::HOME);
    EXPECT_EQ(messages[0], "[Add Subtask]\n");
    EXPECT_EQ(messages[5], "Added Subtask");
    EXPECT_EQ(messages[6], " (ID: 2)\n");
}

TEST_F(StepTest, executeEditStepAllDefault)
{
    std::vector<std::string> scenario = {"", "", ""};
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    auto t = ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                   1767218399, "label", false);
    tm->Add(t);
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>(scenario);
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    std::vector<std::string> expected_prompts = {"    Title [title] > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) [Medium] > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} [Wed Dec 31 23:59:59 2025] > "
    };

    auto step = f->lazyInitStep(Factory::State::EDIT);
    Context c;
    ProtoTask::TaskID id;
    id.set_value(1);
    c.setID(id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    EXPECT_EQ(t, c.task());

    auto prompts = mr->prompts();
    EXPECT_EQ(prompts.size(), expected_prompts.size());
    for (int i = 0; i < prompts.size(); ++i)
        EXPECT_EQ(prompts[i], expected_prompts[i]);

    auto messages = mp->messages();
    EXPECT_EQ(messages[0], "[Edit Task]\n");
    EXPECT_EQ(messages[1], "Edited Task");
    EXPECT_EQ(messages[2], " (ID: 1)\n");
}

TEST_F(StepTest, executeEditStepAllNew)
{
    std::vector<std::string> scenario = {"edited", "10", "low", "3", "31-12-22", "31/12/2022 15:00"};
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    tm->Add(ProtoTask::createTask("original", ProtoTask::Task_Priority_MEDIUM,
                                  1767218399, "label", false));
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>(scenario);
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    std::vector<std::string> expected_prompts = {"    Title [original] > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) [Medium] > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) [Medium] > ",
                                                 "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) [Medium] > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} [Wed Dec 31 23:59:59 2025] > ",
                                                 "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} [Wed Dec 31 23:59:59 2025] > "
    };

    auto step = f->lazyInitStep(Factory::State::EDIT);
    Context c;
    ProtoTask::TaskID id;
    id.set_value(1);
    c.setID(id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour = 15;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    timeinfo->tm_mon = 11;
    timeinfo->tm_mday = 31;
    timeinfo->tm_year = 122;
    ProtoTask::Task t = ProtoTask::createTask("edited", ProtoTask::Task_Priority_HIGH,
                                              mktime(timeinfo), "label", false);
    EXPECT_EQ(t, c.task());

    auto prompts = mr->prompts();
    EXPECT_EQ(prompts.size(), expected_prompts.size());
    for (int i = 0; i < prompts.size(); ++i)
        EXPECT_EQ(prompts[i], expected_prompts[i]);

    auto messages = mp->messages();
    EXPECT_EQ(messages[0], "[Edit Task]\n");
    EXPECT_EQ(messages[1], "    Wrong priority option. Try again.\n");
    EXPECT_EQ(messages[2], "    Wrong priority option. Try again.\n");
    EXPECT_EQ(messages[3], "    Wrong date format. Try again.\n");
    EXPECT_EQ(messages[4], "Edited Task");
    EXPECT_EQ(messages[5], " (ID: 1)\n");
}

TEST_F(StepTest, executeHelpStep)
{
    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp);
    auto step = f->lazyInitStep(Factory::State::HELP);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    FileReader fr("../src/model/help.txt");
    std::string expected_output = fr.read("");
    auto messages = mp->messages();
    EXPECT_EQ(expected_output, messages[0]);
}

TEST_F(StepTest, stringToTimeWithDateOnly)
{
    ReadTaskDataStep step;
    std::optional<time_t> out_time = step.stringToTime("31/12");
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_mday = 31;
    timeinfo->tm_mon = 11;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    time_t expected_time = mktime(timeinfo);
    EXPECT_EQ(*out_time, expected_time);
}

TEST_F(StepTest, stringToTimeWithDateAndYear)
{
    ReadTaskDataStep step;
    std::optional<time_t> out_time = step.stringToTime("31.12.22");
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_mday = 31;
    timeinfo->tm_mon = 11;
    timeinfo->tm_year = 122;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    time_t expected_time = mktime(timeinfo);
    EXPECT_EQ(*out_time, expected_time);
}

TEST_F(StepTest, stringToTimeWithDateAndYearAndTime)
{
    ReadTaskDataStep step;
    std::optional<time_t> out_time = step.stringToTime("31.12.22 15:20");
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_mday = 31;
    timeinfo->tm_mon = 11;
    timeinfo->tm_year = 122;
    timeinfo->tm_hour = 15;
    timeinfo->tm_min = 20;
    timeinfo->tm_sec = 0;
    time_t expected_time = mktime(timeinfo);
    EXPECT_EQ(*out_time, expected_time);
}

TEST_F(StepTest, stringToTimeInFutureTime)
{
    ReadTaskDataStep step;
    std::optional<time_t> out_time = step.stringToTime("in 01:30");
    time_t expected_time = time(nullptr) + 3600 + 30*60;
    EXPECT_EQ(*out_time, expected_time);
}

TEST_F(StepTest, stringToTimeInFutureDayAndTime)
{
    ReadTaskDataStep step;
    std::optional<time_t> out_time = step.stringToTime("in 02:02:02");
    time_t expected_time = time(nullptr) + 2*24*3600 + 2*3600 + 2*60;
    EXPECT_EQ(*out_time, expected_time);
}

TEST_F(StepTest, executeQuitStep)
{
    auto f = Factory::create();
    auto step = f->lazyInitStep(Factory::State::QUIT);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, nullptr);
}

TEST_F(StepTest, executeShowStep)
{
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    ActionResult result = tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  1767218399, "label", false));
    tm->AddSubtask(ProtoTask::createTask("subtitle", ProtoTask::Task_Priority_HIGH,
                                         1767218399, "sub_label", true), *result.id);
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    auto step = f->lazyInitStep(Factory::State::SHOW);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    auto messages = mp->messages();
    EXPECT_EQ(messages[0], "1 – title, Priority: Medium, Due: Wed Dec 31 23:59:59 2025 L: label");
    EXPECT_EQ(messages[2], "    2 – subtitle, Priority: High, Due: Wed Dec 31 23:59:59 2025 L: sub_label [completed]");
}

TEST_F(StepTest, executeCompleteStep)
{
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  1767218399, "label", false));
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    auto step = f->lazyInitStep(Factory::State::COMPLETE);
    Context c;
    ProtoTask::TaskID id;
    id.set_value(1);
    c.setID(id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    EXPECT_TRUE((*tm)[id].first.is_complete());

    auto messages = mp->messages();
    EXPECT_EQ(messages[0], "Completed Task");
    EXPECT_EQ(messages[1], " (ID: 1)\n");
}

TEST_F(StepTest, executeUncompleteStep)
{
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  1767218399, "label", true));
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    auto step = f->lazyInitStep(Factory::State::UNCOMPLETE);
    Context c;
    ProtoTask::TaskID id;
    id.set_value(1);
    c.setID(id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    EXPECT_FALSE((*tm)[id].first.is_complete());

    auto messages = mp->messages();
    EXPECT_EQ(messages[0], "Uncompleted Task");
    EXPECT_EQ(messages[1], " (ID: 1)\n");
}

TEST_F(StepTest, executeDeleteStep)
{
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  1767218399, "label", false));
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    auto step = f->lazyInitStep(Factory::State::DELETE);
    Context c;
    ProtoTask::TaskID id;
    id.set_value(1);
    c.setID(id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    EXPECT_TRUE(tm->getTasks().empty());

    auto messages = mp->messages();
    EXPECT_EQ(messages[0], "Deleted Task");
    EXPECT_EQ(messages[1], " (ID: 1)\n");
}

void processConfirmDeleteCall(const std::vector<std::string> &scenario, Factory::State expected_step) {
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    ActionResult result = tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                                        1767218399, "label", false));
    tm->AddSubtask(ProtoTask::createTask("subtitle", ProtoTask::Task_Priority_HIGH,
                                         1767218399, "sub_label", true), *result.id);
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReaderToVector>(scenario);
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    auto step = f->lazyInitStep(Factory::State::CONFIRMDELETE);
    Context c;
    c.setID(result.id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(expected_step));

    auto prompts = mr->prompts();
    EXPECT_EQ(prompts[0], "Task 1 has 1 subtask(s). Confirm to delete all. Y/[N] > ");
    auto messages = mp->messages();
    if (!messages.empty()) {
        EXPECT_EQ(messages[0], "Wrong option. Type Y or N.\n");
    }
}

TEST_F(StepTest, executeConfirmDeleteStepWithEmptyStr)
{
    processConfirmDeleteCall(std::vector<std::string>{""}, Factory::State::HOME);
}

TEST_F(StepTest, processConfirmDeleteStepWithYStr)
{
    processConfirmDeleteCall(std::vector<std::string>{"Y"}, Factory::State::DELETE);
}

TEST_F(StepTest, processConfirmDeleteStepWithNStr)
{
    processConfirmDeleteCall(std::vector<std::string>{"N"}, Factory::State::HOME);
}

TEST_F(StepTest, processConfirmDeleteStepWithBadStr)
{
    processConfirmDeleteCall(std::vector<std::string>{"bad", "n"}, Factory::State::HOME);
}

TEST_F(StepTest, processConfirmDeleteStepIDNotFound)
{
    ProtoTask::TaskID id;
    id.set_value(42);
    ActionResult result{ActionResult::Status::ID_NOT_FOUND, id};
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReaderToVector),
                                               std::shared_ptr<AbstractPrinter>(new MockPrinterToVector),
                                               std::shared_ptr<ControllerInterface>(new MockController));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockController>(f->controller()), ReadTaskWithChildren)
            .WillOnce(Return(result));
    auto step = f->lazyInitStep(Factory::State::CONFIRMDELETE);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    auto messages = std::dynamic_pointer_cast<MockPrinterToVector>(f->printer())->messages();
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ(messages[0], "ID 42 was not found.\n");

}

TEST_F(StepTest, executeLabelStep)
{
    auto gen = std::make_shared<IDGenerator>(1);
    auto tm = std::make_shared<TaskManager>(gen);
    ActionResult result = tm->Add(ProtoTask::createTask("title", ProtoTask::Task_Priority_MEDIUM,
                                  1767218399, "", false));
    auto con = std::make_shared<Controller>(tm);
    std::vector<std::string> scenario {"", "label"};
    auto mr = std::make_shared<MockReaderToVector>(scenario);
    auto mp = std::make_shared<MockPrinterToVector>();
    auto f = Factory::create(mr, mp, con);

    auto step = f->lazyInitStep(Factory::State::LABEL);
    Context c;
    c.setID(result.id);
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    auto prompts = mr->prompts();
    ASSERT_EQ(2, prompts.size());
    EXPECT_EQ(prompts[0], "[Add Label]\n    >> ");
    EXPECT_EQ(prompts[1], "Label cannot be empty. Try again.\n    >> ");
    auto messages = mp->messages();
    ASSERT_EQ(2, messages.size());
    EXPECT_EQ(messages[0], "Added label to Task");
    EXPECT_EQ(messages[1], " (ID: 1)\n");
}

std::vector<std::string> executeLoadStep(ActionResult::Status s) {
    ActionResult success_result{s, std::nullopt};

    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto con = std::make_shared<MockController>();
    auto f = Factory::create(mr, mp, con);

    EXPECT_CALL(*con, LoadTasks)
            .WillOnce(Return(success_result));
    auto step = f->lazyInitStep(Factory::State::LOAD);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    return mp->messages();
}

TEST_F(StepTest, shouldExecuteLoadStepSuccessfully)
{
    auto messages = executeLoadStep(ActionResult::Status::SUCCESS);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("Loaded from file successfully.\n", messages[0]);
}

TEST_F(StepTest, shouldExecuteLoadStepFileNotFound)
{
    auto messages = executeLoadStep(ActionResult::Status::FILE_NOT_FOUND);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("File could not be found.\n", messages[0]);
}

std::vector<std::string> executeSaveStep(ActionResult::Status s) {
    ActionResult success_result{s, std::nullopt};

    auto mr = std::make_shared<MockReaderToVector>();
    auto mp = std::make_shared<MockPrinterToVector>();
    auto con = std::make_shared<MockController>();
    auto f = Factory::create(mr, mp,con);
    EXPECT_CALL(*con, SaveTasks)
            .WillOnce(Return(success_result));

    auto step = f->lazyInitStep(Factory::State::SAVE);
    Context c;
    auto next_step = step->execute(c, f);
    EXPECT_EQ(next_step, f->lazyInitStep(Factory::State::HOME));

    return mp->messages();
}

TEST_F(StepTest, shouldExecuteSaveStepSuccessfully)
{
    auto messages = executeSaveStep(ActionResult::Status::SUCCESS);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("Saved to file successfully.\n", messages[0]);
}

TEST_F(StepTest, shouldExecuteSaveStepFailedToOpen)
{
    auto messages = executeSaveStep(ActionResult::Status::FAILED_TO_OPEN_FILE);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ("Failed to open file.\n", messages[0]);
}
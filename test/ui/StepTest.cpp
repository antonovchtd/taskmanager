//
// Created by Anton O. on 12/4/21.
//

#include "../../src/ui/Step.h"
#include "../../src/ui/Context.h"

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

void homeStepExecuteCall(const std::string &command, Factory::ActionLabel a) {
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
        .Times(1)
        .WillOnce(Return(command));

    auto step = f->lazyInitStep(Factory::State::HOME);
    Context c;
    auto action = step->execute(c);
    EXPECT_EQ(action, f->lazyInitAction(a));
    std::istringstream is{command};
    std::string function, argument;
    is >> function >> argument;
    EXPECT_EQ(action->data().arg, argument);
}

void executeWithWizardCall(Factory::State s, Factory::ActionLabel a) {
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
        .Times(7)
        .WillOnce(Return(""))
        .WillOnce(Return("test"))
        .WillOnce(Return("10"))
        .WillOnce(Return("low"))
        .WillOnce(Return("2"))
        .WillOnce(Return("31-12-22"))
        .WillOnce(Return("31/12/2022 15:00"));

    std::vector<std::string> out(5);
    int i = 0;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
        .Times(5)
        .WillOnce(SaveArg<0>(&out[i++]))
        .WillOnce(SaveArg<0>(&out[i++]))
        .WillOnce(SaveArg<0>(&out[i++]))
        .WillOnce(SaveArg<0>(&out[i++]))
        .WillOnce(SaveArg<0>(&out[i++]));

    auto step = f->lazyInitStep(s);
    Context c;
    auto action = step->execute(c);

    EXPECT_EQ(out[1], "    Title cannot be empty!\n");
    EXPECT_EQ(out[2], "    Wrong priority option. Try again.\n");
    EXPECT_EQ(out[3], "    Wrong priority option. Try again.\n");
    EXPECT_EQ(out[4], "    Wrong date format. Try again.\n");
    EXPECT_EQ(c.data().title, "test");
    EXPECT_EQ(c.data().priority, Task::Priority::MEDIUM);
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour = 15;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    timeinfo->tm_mon = 11;
    timeinfo->tm_mday = 31;
    timeinfo->tm_year = 122;
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));
    EXPECT_EQ(action, f->lazyInitAction(a));
}

std::string processCallAckID(Factory::State s, Factory::State next, Context &c) {
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
        .Times(1)
        .WillRepeatedly(SaveArg<0>(&out));

    auto step = f->lazyInitStep(s);
    c.setID(TaskID(42));
    step->process(c);
    EXPECT_EQ(c.step(), f->lazyInitStep(next));
    return out;
}

void processGetNextStep(Factory::State s, Factory::State next) {
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
        .Times(0)
        .WillOnce(SaveArg<0>(&out));

    auto step = f->lazyInitStep(s);
    Context c;
    step->process(c);
    EXPECT_EQ(out, "");
    EXPECT_EQ(c.step(), f->lazyInitStep(next));
}

void executeGetActionCall(Factory::State s, Factory::ActionLabel a) {
    Context c;
    auto f = Factory::create();
    auto step = f->lazyInitStep(s);
    auto action = step->execute(c);
    EXPECT_EQ(action, f->lazyInitAction(a));
}

void processConfirmDeleteCall(const std::string &reply, Factory::State s) {
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(DoAll(SaveArg<0>(&out), Return(reply)));

    auto step = f->lazyInitStep(Factory::State::CONFIRMDELETE);
    Context c;
    TaskManager tm;
    Task t = Task::Create("test", Task::Priority::NONE, time(nullptr), "label", false);
    TaskID id = tm.Add(t);
    tm.AddSubtask(t, id);
    c.setTasks(tm.getTasks());
    c.setID(id);
    step->process(c);

    EXPECT_EQ(out, "Task " + c.id().value().to_string() +
                   " has " + std::to_string(tm[id].second.children().size()) +
                   " subtask(s). Confirm to delete all. [Y]/N > ");
    EXPECT_EQ(c.step(), f->lazyInitStep(s));
}

TEST_F(StepTest, executeHomeStepWithAddCommand)
{
    homeStepExecuteCall("add", Factory::ActionLabel::VALIDATENOID);
}

TEST_F(StepTest, executeHomeStepWithHelpCommand)
{
    homeStepExecuteCall("help", Factory::ActionLabel::VALIDATENOID);
}

TEST_F(StepTest, executeHomeStepWithQuitCommand)
{
    homeStepExecuteCall("quit", Factory::ActionLabel::VALIDATENOID);
}

TEST_F(StepTest, executeHomeStepWithShowCommand)
{
    homeStepExecuteCall("show", Factory::ActionLabel::VALIDATELABEL);
}

TEST_F(StepTest, executeHomeStepWithEditCommand)
{
    homeStepExecuteCall("edit", Factory::ActionLabel::VALIDATEID);
}

TEST_F(StepTest, executeHomeStepWithEditCommandWithID)
{
    homeStepExecuteCall("edit 1", Factory::ActionLabel::VALIDATEID);
}

TEST_F(StepTest, executeHomeStepWithSubtaskCommand)
{
    homeStepExecuteCall("subtask 1", Factory::ActionLabel::VALIDATEID);
}

TEST_F(StepTest, executeHomeStepWithCompleteCommand)
{
    homeStepExecuteCall("complete 1", Factory::ActionLabel::VALIDATEID);
}

TEST_F(StepTest, executeHomeStepWithDeleteCommand)
{
    homeStepExecuteCall("delete 2", Factory::ActionLabel::VALIDATEID);
}

TEST_F(StepTest, executeHomeStepWithLabelCommand)
{
    homeStepExecuteCall("label 1", Factory::ActionLabel::VALIDATEID);
}


TEST_F(StepTest, executeHomeStepWithEmptyCommand)
{
    homeStepExecuteCall("", Factory::ActionLabel::VALIDATENOID);
}

TEST_F(StepTest, executeHomeStepWithBadCommand)
{
    homeStepExecuteCall("bad", Factory::ActionLabel::VALIDATENOID);
}

TEST_F(StepTest, processHomeStepWithMissingIDPrintsMessage)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));
    auto hs = f->lazyInitStep(Factory::State::HOME);
    Context c;
    c.setID(std::nullopt);
    hs->process(c);
    EXPECT_EQ(out, "This function takes no argument.\n");
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::HOME));
}

TEST_F(StepTest, processHomeStepWithInvalidIDPrintsMessage)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));
    auto hs = f->lazyInitStep(Factory::State::HOME);
    Context c;
    c.setID(TaskID::invalidID());
    hs->process(c);
    EXPECT_EQ(out, "Invalid ID. Try again.\n");
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::HOME));
}

TEST_F(StepTest, processHomeStepWithValidIDPrintsNoMessage)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(Return("edit"));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(0)
            .WillOnce(SaveArg<0>(&out));

    auto step = f->lazyInitStep(Factory::State::HOME);
    Context c;
    step->execute(c); // record command_ property
    c.setID(TaskID(1));
    step->process(c);
    EXPECT_EQ(out, "");
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::EDIT));
}

TEST_F(StepTest, shouldExecuteHelpStep)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));

    auto hs = f->lazyInitStep(Factory::State::HELP);
    Context c;
    auto action = hs->execute(c);
    EXPECT_EQ(action, f->lazyInitAction(Factory::ActionLabel::DONOTHING));

    std::ifstream file("../src/model/help.txt");
    std::istringstream is{out};
    std::string line, lineOut;
    while (getline(file, line)){
        getline(is, lineOut);
        EXPECT_EQ(line, lineOut);
    }
}

TEST_F(StepTest, processHelpStepPrintNothing)
{
    processGetNextStep(Factory::State::HELP, Factory::State::HOME);
}

TEST_F(StepTest, executeAddStepWithFutureDate)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out1, out2, out3;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(3)
            .WillOnce(DoAll(SaveArg<0>(&out1), Return("test")))
            .WillOnce(DoAll(SaveArg<0>(&out2), Return("1")))
            .WillOnce(DoAll(SaveArg<0>(&out3), Return("31/12")));

    std::string out_printer;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out_printer));

    auto step = f->lazyInitStep(Factory::State::ADD);
    Context c;
    auto action = step->execute(c);

    EXPECT_EQ(out_printer, "[Add Task]\n");
    EXPECT_EQ(out1, "    Title > ");
    EXPECT_EQ(out2, "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ");
    EXPECT_EQ(out3, "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ");
    EXPECT_EQ(c.data().title, "test");
    EXPECT_EQ(c.data().priority, Task::Priority::LOW);
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    timeinfo->tm_mon = 11;
    timeinfo->tm_mday = 31;
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));
    EXPECT_EQ(action, f->lazyInitAction(Factory::ActionLabel::ADDTASK));
}

TEST_F(StepTest, executeAddStepWithFutureDateAndTime)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out1, out2, out3;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(3)
            .WillOnce(DoAll(SaveArg<0>(&out1), Return("test")))
            .WillOnce(DoAll(SaveArg<0>(&out2), Return("3")))
            .WillOnce(DoAll(SaveArg<0>(&out3), Return("31/12/21 23:45")));

    std::string out_printer;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out_printer));

    auto step = f->lazyInitStep(Factory::State::ADD);
    Context c;
    auto action = step->execute(c);

    EXPECT_EQ(out_printer, "[Add Task]\n");
    EXPECT_EQ(out1, "    Title > ");
    EXPECT_EQ(out2, "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ");
    EXPECT_EQ(out3, "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ");
    EXPECT_EQ(c.data().title, "test");
    EXPECT_EQ(c.data().priority, Task::Priority::HIGH);
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour = 23;
    timeinfo->tm_min = 45;
    timeinfo->tm_sec = 0;
    timeinfo->tm_mon = 11;
    timeinfo->tm_mday = 31;
    timeinfo->tm_year = 121;
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));
    EXPECT_EQ(action, f->lazyInitAction(Factory::ActionLabel::ADDTASK));
}

TEST_F(StepTest, executeAddStepWithFutureTime)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out1, out2, out3;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(3)
            .WillOnce(DoAll(SaveArg<0>(&out1), Return("test")))
            .WillOnce(DoAll(SaveArg<0>(&out2), Return("2")))
            .WillOnce(DoAll(SaveArg<0>(&out3), Return("in 01:00")));

    std::string out_printer;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out_printer));

    auto step = f->lazyInitStep(Factory::State::ADD);
    Context c;
    auto action = step->execute(c);

    EXPECT_EQ(out_printer, "[Add Task]\n");
    EXPECT_EQ(out1, "    Title > ");
    EXPECT_EQ(out2, "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ");
    EXPECT_EQ(out3, "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > ");
    EXPECT_EQ(c.data().title, "test");
    EXPECT_EQ(c.data().priority, Task::Priority::MEDIUM);

    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour += 1;
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));
    EXPECT_EQ(action, f->lazyInitAction(Factory::ActionLabel::ADDTASK));
}

TEST_F(StepTest, executeAddStepWithMissingEntries)
{
    executeWithWizardCall(Factory::State::ADD, Factory::ActionLabel::ADDTASK);
}

TEST_F(StepTest, processAddStep)
{
    Context c;
    std::string out = processCallAckID(Factory::State::ADD, Factory::State::HOME, c);
    EXPECT_EQ(out, "Added Task with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, processReadTaskDataStepPrintNothing)
{
    processGetNextStep(Factory::State::READTASK, Factory::State::QUIT);
}

TEST_F(StepTest, executeEditStep)
{
    executeWithWizardCall(Factory::State::EDIT, Factory::ActionLabel::EDIT);
}

TEST_F(StepTest, processEditStep)
{
    Context c;
    std::string out = processCallAckID(Factory::State::EDIT, Factory::State::HOME, c);
    EXPECT_EQ(out, "Edited Task with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, executeSubtaskStep)
{
    executeWithWizardCall(Factory::State::SUBTASK, Factory::ActionLabel::ADDSUBTASK);
}

TEST_F(StepTest, processSubtaskStep)
{
    Context c;
    std::string out = processCallAckID(Factory::State::SUBTASK, Factory::State::HOME, c);
    EXPECT_EQ(out, "Added Subtask with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, executeShowStep)
{
    executeGetActionCall(Factory::State::SHOW, Factory::ActionLabel::SHOW);
}

TEST_F(StepTest, processShowStep)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1))
            .WillOnce(SaveArg<0>(&out));

    TaskManager tm;
    tm.Add(Task::Create("test", Task::Priority::MEDIUM, 1767218399, "", false));

    auto ss = f->lazyInitStep(Factory::State::SHOW);
    Context c;
    c.setTasks(tm.getTasks());
    ss->process(c);
    EXPECT_EQ(out, "1 – test, Priority: Medium, Due: Wed Dec 31 23:59:59 2025");
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::HOME));
}

TEST_F(StepTest, processShowStepWithChildren)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out1, out2, out3;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1))
            .WillOnce(SaveArg<0>(&out1))
            .WillOnce(SaveArg<0>(&out2))
            .WillOnce(SaveArg<0>(&out3));

    TaskManager tm;
    auto id = tm.Add(Task::Create("test", Task::Priority::MEDIUM, 1767218399, "a", false));
    tm.AddSubtask(Task::Create("sub", Task::Priority::LOW, 1767218399, "b", false), id);

    auto ss = f->lazyInitStep(Factory::State::SHOW);
    Context c;
    c.setTasks(tm.getTasks());
    ss->process(c);
    EXPECT_EQ(out1, "1 – test, Priority: Medium, Due: Wed Dec 31 23:59:59 2025 L: a");
    EXPECT_EQ(out2, "\n");
    EXPECT_EQ(out3, "    2 – sub, Priority: Low, Due: Wed Dec 31 23:59:59 2025 L: b");
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::HOME));
}

TEST_F(StepTest, executeCompleteStep)
{
    executeGetActionCall(Factory::State::COMPLETE, Factory::ActionLabel::COMPLETE);
}

TEST_F(StepTest, processCompleteStep)
{
    Context c;
    std::string out = processCallAckID(Factory::State::COMPLETE, Factory::State::HOME, c);
    EXPECT_EQ(out, "Marked Task with ID " + c.id().value().to_string() + " as completed.\n");
}

TEST_F(StepTest, executeDeleteStep)
{
    executeGetActionCall(Factory::State::DELETE, Factory::ActionLabel::DELETE);
}

TEST_F(StepTest, processDeleteStep)
{
    Context c;
    std::string out = processCallAckID(Factory::State::DELETE, Factory::State::HOME, c);
    EXPECT_EQ(out, "Deleted Task with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, processConfirmDeleteStepWithEmptyStr)
{
    processConfirmDeleteCall("", Factory::State::DELETE);
}

TEST_F(StepTest, processConfirmDeleteStepWithYStr)
{
    processConfirmDeleteCall("Y", Factory::State::DELETE);
}

TEST_F(StepTest, processConfirmDeleteStepWithNStr)
{
    processConfirmDeleteCall("N", Factory::State::HOME);
}

TEST_F(StepTest, processConfirmDeleteStepWithBadStr)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));

    std::string out, out2;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(2)
            .WillOnce(DoAll(SaveArg<0>(&out), Return("bad")))
            .WillOnce(Return("n"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out2));

    auto step = f->lazyInitStep(Factory::State::CONFIRMDELETE);
    Context c;
    TaskManager tm;
    Task t = Task::Create("test", Task::Priority::NONE, time(nullptr), "label", false);
    TaskID id = tm.Add(t);
    tm.AddSubtask(t, id);
    c.setTasks(tm.getTasks());
    c.setID(id);
    step->process(c);

    EXPECT_EQ(out, "Task " + c.id().value().to_string() +
                   " has " + std::to_string(tm[id].second.children().size()) +
                   " subtask(s). Confirm to delete all. [Y]/N > ");
    EXPECT_EQ(out2, "Wrong option. Type Y or N.\n");
    EXPECT_EQ(c.step(), f->lazyInitStep(Factory::State::HOME));
}

TEST_F(StepTest, executeLabelStep)
{
    MockReader mr;
    MockPrinter mp;
    auto f = Factory::create(std::shared_ptr<AbstractReader>(&mr),
                             std::shared_ptr<AbstractPrinter>(&mp));
    std::string label = "myLabel", out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(DoAll(SaveArg<0>(&out), Return(label)));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(0);

    auto step = f->lazyInitStep(Factory::State::LABEL);
    Context c;
    auto action = step->execute(c);

    EXPECT_EQ(action, f->lazyInitAction(Factory::ActionLabel::LABEL));
    EXPECT_EQ(out, "[Add Label]\n    >> ");
    EXPECT_EQ(label, action->data().arg);
}

TEST_F(StepTest, processLabelStep)
{
    Context c;
    std::string out = processCallAckID(Factory::State::LABEL, Factory::State::HOME, c);
    EXPECT_EQ(out, "Added label to Task with ID " + c.id().value().to_string() + ".\n");
}
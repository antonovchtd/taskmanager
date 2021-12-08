//
// Created by Anton O. on 12/4/21.
//

#include "../../src/ui/Step.h"
#include "../../src/controller/Action.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using testing::SaveArg;

#define STEPTEST_GEN_HOMESTEP_CALL(command, act, step) \
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader), \
              std::shared_ptr<AbstractPrinter>(new MockPrinter)); \
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_)) \
        .Times(AtLeast(1)) \
        .WillOnce(Return(#command))                     \
        .WillRepeatedly(Return("")); \
    auto hs = f->getHomeStep(); \
    Context c; \
    auto action = hs->execute(c); \
    EXPECT_EQ(action, f->act()); \
    EXPECT_EQ(c.getStep(), f->step()); \
    EXPECT_EQ(c.getOldStep(), nullptr);

#define STEPTEST_GEN_WIZARD_STEP_CALL(getter, act) \
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),\
        std::shared_ptr<AbstractPrinter>(new MockPrinter));\
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))\
        .Times(6)\
        .WillOnce(Return(""))\
        .WillOnce(Return("test"))\
        .WillOnce(Return("10"))\
        .WillOnce(Return("2"))\
        .WillOnce(Return("31-12-22"))\
        .WillOnce(Return("31/12/2022 15:00"));\
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(4);\
    auto hs = f->getter();\
    Context c;\
    auto action = hs->execute(c);\
    EXPECT_EQ(c.data().title, "test");\
    EXPECT_EQ(c.data().priority, Task::Priority::MEDIUM);\
    time_t rawtime;\
    time(&rawtime);\
    struct tm * timeinfo = localtime(&rawtime);\
    timeinfo->tm_hour = 15;\
    timeinfo->tm_min = 0;\
    timeinfo->tm_sec = 0;\
    timeinfo->tm_mon = 11;\
    timeinfo->tm_mday = 31;\
    timeinfo->tm_year = 122;\
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));\
    EXPECT_EQ(action, f->act());\
    EXPECT_EQ(c.getStep(), f->getHomeStep());\

#define STEPTEST_GEN_PROCESS_CALL_ACK_ID(getter)\
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),\
              std::shared_ptr<AbstractPrinter>(new MockPrinter));\
    std::string out;\
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(AtLeast(1))\
        .WillRepeatedly(SaveArg<0>(&out));\
    auto hs = f->getter();\
    Context c;\
    c.setID(TaskID(42));\
    hs->process(c);\

#define STEPTEST_GEN_PROCESS_DO_NOTHING(getter) \
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),\
          std::shared_ptr<AbstractPrinter>(new MockPrinter));\
    std::string out;\
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(0)\
        .WillOnce(SaveArg<0>(&out));\
    auto hs = f->getter();\
    Context c;\
    hs->process(c);

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

TEST_F(StepTest, executeHomeStepWithAddCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(add, getValidateNoArgAction, getAddStep)
}

TEST_F(StepTest, executeHomeStepWithHelpCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(help, getValidateNoArgAction, getHelpStep)
}

TEST_F(StepTest, executeHomeStepWithQuitCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(quit, getValidateNoArgAction, getQuitStep)
}

TEST_F(StepTest, executeHomeStepWithShowCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(show, getValidateLabelArgAction, getShowStep)
}

TEST_F(StepTest, executeHomeStepWithEditCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(edit, getValidateIDAction, getEditStep)
}

TEST_F(StepTest, executeHomeStepWithEditCommandWithID)
{
    STEPTEST_GEN_HOMESTEP_CALL(edit 1, getValidateIDAction, getEditStep)
    EXPECT_EQ(c.arg(), "1");
}

TEST_F(StepTest, executeHomeStepWithSubtaskCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(subtask, getValidateIDAction, getSubtaskStep)
}

TEST_F(StepTest, executeHomeStepWithCompleteCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(complete, getValidateIDAction, getCompleteStep)
}

TEST_F(StepTest, executeHomeStepWithDeleteCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(delete, getValidateIDAction, getConfirmDeleteStep)
}

TEST_F(StepTest, executeHomeStepWithLabelCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(label, getValidateIDAction, getLabelStep)
}


TEST_F(StepTest, executeHomeStepWithEmptyCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(, getValidateNoArgAction, getHomeStep)
}

TEST_F(StepTest, executeHomeStepWithBadCommand)
{
//    STEPTEST_GEN_HOMESTEP_CALL(bad, getValidateNoArgAction, getHomeStep)
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
        .Times(1)
        .WillOnce(Return("bad"));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));
    auto hs = f->getHomeStep();
    Context c;
    auto action = hs->execute(c);
    EXPECT_EQ(action, f->getValidateNoArgAction());
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(c.getOldStep(), nullptr);
    EXPECT_EQ(out, "Wrong command. Try again. Type `help` for help.\n");
}

TEST_F(StepTest, processHomeStepWithMissingIDPrintsMessage)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));
    auto hs = f->getHomeStep();
    Context c;
    c.setID(std::nullopt);
    hs->process(c);
    EXPECT_EQ(out, "This function takes no argument.\n");
}

TEST_F(StepTest, processHomeStepWithInvalidIDPrintsMessage)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));
    auto hs = f->getHomeStep();
    Context c;
    c.setID(TaskID::invalidID());
    hs->process(c);
    EXPECT_EQ(out, "Invalid ID. Try again.\n");
}

TEST_F(StepTest, processHomeStepWithValidIDPrintsNoMessage)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(0)
            .WillOnce(SaveArg<0>(&out));

    auto hs = f->getHomeStep();
    Context c;
    c.setID(TaskID(1));
    hs->process(c);
}

TEST_F(StepTest, shouldExecuteHelpStep)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1)
            .WillOnce(SaveArg<0>(&out));

    auto hs = f->getHelpStep();
    Context c;
    auto action = hs->execute(c);
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(action, nullptr);

    std::ifstream file("../src/model/help.txt");
    std::istringstream is{out};
    std::string line, lineOut;
    while (getline(file, line)){
        getline(is, lineOut);
        EXPECT_EQ(line, lineOut);
    }
}

TEST_F(StepTest, shouldRecordOldStep)
{
    STEPTEST_GEN_HOMESTEP_CALL(help, getValidateNoArgAction, getHelpStep)
    action = hs->execute(c);
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(c.getOldStep(), f->getHelpStep());
}

TEST_F(StepTest, processHelpStepPrintNothing)
{
    STEPTEST_GEN_PROCESS_DO_NOTHING(getHelpStep)
}

TEST_F(StepTest, executeAddStepWithFutureDate)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(3)
            .WillOnce(Return("test"))
            .WillOnce(Return("1"))
            .WillOnce(Return("31/12"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1);

    auto hs = f->getAddStep();
    Context c;
    auto action = hs->execute(c);
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
    EXPECT_EQ(action, f->getAddTaskAction());
    EXPECT_EQ(c.getStep(), f->getHomeStep());
}

TEST_F(StepTest, executeAddStepWithFutureDateAndTime)
{

    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(3)
            .WillOnce(Return("test"))
            .WillOnce(Return("3"))
            .WillOnce(Return("31/12/21 23:45"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1);

    auto hs = f->getAddStep();
    Context c;
    auto action = hs->execute(c);
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
    EXPECT_EQ(action, f->getAddTaskAction());
    EXPECT_EQ(c.getStep(), f->getHomeStep());
}

TEST_F(StepTest, executeAddStepWithFutureTime)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(3)
            .WillOnce(Return("test"))
            .WillOnce(Return("2"))
            .WillOnce(Return("in 01:00"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(1);

    auto hs = f->getAddStep();
    Context c;
    auto action = hs->execute(c);

    EXPECT_EQ(c.data().title, "test");
    EXPECT_EQ(c.data().priority, Task::Priority::MEDIUM);

    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour += 1;
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));
    EXPECT_EQ(action, f->getAddTaskAction());
    EXPECT_EQ(c.getStep(), f->getHomeStep());
}

TEST_F(StepTest, executeAddStepWithMissingEntries)
{
    STEPTEST_GEN_WIZARD_STEP_CALL(getAddStep, getAddTaskAction)
}

TEST_F(StepTest, processAddStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(getAddStep)
    EXPECT_EQ(out, "Added Task with ID 42.\n");

    // test that process() resets data in context
    c.setData(Task::Data{"title", Task::Priority::NONE, time(nullptr), false});
    c.setID(TaskID(1));
    hs->process(c);
    EXPECT_TRUE(c.data().title.empty());
    EXPECT_EQ(c.id(), TaskID::invalidID());

}

TEST_F(StepTest, getValidateArgActionReadTaskDataStep)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    auto rtds = f->getReadTaskDataStep();
    auto action = rtds->getValidateArgAction();
    EXPECT_EQ(action, f->getValidateNoArgAction());
}

TEST_F(StepTest, processReadTaskDataStepPrintNothing)
{
    STEPTEST_GEN_PROCESS_DO_NOTHING(getReadTaskDataStep)
}

TEST_F(StepTest, executeEditStep)
{
    STEPTEST_GEN_WIZARD_STEP_CALL(getEditStep, getEditAction)
}

TEST_F(StepTest, processEditStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(getEditStep)
    EXPECT_EQ(out, "Edited Task with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, executeSubtaskStep)
{
    STEPTEST_GEN_WIZARD_STEP_CALL(getSubtaskStep, getAddSubtaskAction)
}

TEST_F(StepTest, processSubtaskStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(getSubtaskStep)
    EXPECT_EQ(out, "Added Subtask with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, processQuitStepPrintNothing)
{
    STEPTEST_GEN_PROCESS_DO_NOTHING(getQuitStep)
}

TEST_F(StepTest, executeShowStep)
{
    Context c;
    auto f = Factory::create();
    auto ss = f->getShowStep();
    auto action = ss->execute(c);
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(action, f->getShowAction());
}

TEST_F(StepTest, processShowStep)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1))
            .WillOnce(SaveArg<0>(&out));

    TaskManager tm;
    tm.Add(Task::Create("test", Task::Priority::MEDIUM, 1767218399, false));

    auto ss = f->getShowStep();
    Context c;
    c.setTasks(tm.getTasks());
    ss->process(c);
    EXPECT_EQ(out, "1 – test, Priority: Medium, Due: Wed Dec 31 23:59:59 2025");
}

TEST_F(StepTest, processShowStepWithChildren)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    std::string out1, out2, out3;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(AtLeast(1))
            .WillOnce(SaveArg<0>(&out1))
            .WillOnce(SaveArg<0>(&out2))
            .WillOnce(SaveArg<0>(&out3));

    TaskManager tm;
    auto id = tm.Add(Task::Create("test", Task::Priority::MEDIUM, 1767218399, false));
    tm.AddSubtask(Task::Create("sub", Task::Priority::LOW, 1767218399, false), id);

    auto ss = f->getShowStep();
    Context c;
    c.setTasks(tm.getTasks());
    ss->process(c);
    EXPECT_EQ(out1, "1 – test, Priority: Medium, Due: Wed Dec 31 23:59:59 2025");
    EXPECT_EQ(out3, "    2 – sub, Priority: Low, Due: Wed Dec 31 23:59:59 2025");
}

TEST_F(StepTest, executeCompleteStep)
{
    Context c;
    auto f = Factory::create();
    auto cs = f->getCompleteStep();
    auto action = cs->execute(c);
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(action, f->getCompleteAction());
}

TEST_F(StepTest, processCompleteStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(getCompleteStep)
    EXPECT_EQ(out, "Marked Task with ID " + c.id().value().to_string() + " as completed.\n");
}

TEST_F(StepTest, executeDeleteStep)
{
    Context c;
    auto f = Factory::create();
    auto ds = f->getDeleteStep();
    auto action = ds->execute(c);
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(action, f->getDeleteAction());
}

TEST_F(StepTest, processDeleteStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(getDeleteStep)
    EXPECT_EQ(out, "Deleted Task with ID " + c.id().value().to_string() + ".\n");
}

TEST_F(StepTest, processConfirmDeleteStepWithConfrimFalse)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(0);

    auto hs = f->getConfirmDeleteStep();
    Context c;
    c.setAskConfirmation(false);
    hs->process(c);
}

TEST_F(StepTest, processConfirmDeleteStepWithConfrimTrueEmptyStr)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(Return(""));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(0);

    auto hs = f->getConfirmDeleteStep();
    Context c;
    c.setAskConfirmation(true);
    c.setID(TaskID(42));
    hs->process(c);

    EXPECT_EQ(c.getStep(), f->getDeleteStep());
    EXPECT_EQ(c.getOldStep(), f->getHomeStep());
}

TEST_F(StepTest, processConfirmDeleteStepWithConfrimTrueYStr)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(Return("Y"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(0);

    auto hs = f->getConfirmDeleteStep();
    Context c;
    c.setAskConfirmation(true);
    c.setID(TaskID(42));
    hs->process(c);

    EXPECT_EQ(c.getStep(), f->getDeleteStep());
    EXPECT_EQ(c.getOldStep(), f->getHomeStep());
}

TEST_F(StepTest, processConfirmDeleteStepWithConfrimTrueNStr)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(Return("N"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(0);

    auto hs = f->getConfirmDeleteStep();
    Context c;
    c.setAskConfirmation(true);
    c.setID(TaskID(42));
    hs->process(c);

    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(c.getOldStep(), f->getHomeStep());
}

TEST_F(StepTest, processConfirmDeleteStepWithConfrimTrueBadStr)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(2)
            .WillOnce(Return("Bad"))
            .WillOnce(Return("N"));

    std::string out;
    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))\
        .Times(1)
        .WillOnce(SaveArg<0>(&out));

    auto hs = f->getConfirmDeleteStep();
    Context c;
    c.setAskConfirmation(true);
    c.setID(TaskID(42));
    hs->process(c);

    EXPECT_EQ(out, "Wrong option. Type Y or N.\n");
    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(c.getOldStep(), f->getHomeStep());
}

TEST_F(StepTest, executeLabelStep)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
              std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read(_))
            .Times(1)
            .WillOnce(Return("myLabel"));

    EXPECT_CALL(*std::dynamic_pointer_cast<MockPrinter>(f->printer()), print(_))
            .Times(0);

    auto hs = f->getLabelStep();
    Context c;
    auto action = hs->execute(c);

    EXPECT_EQ(c.getStep(), f->getHomeStep());
    EXPECT_EQ(action, f->getLabelAction());
    EXPECT_EQ("myLabel", c.label());
}

TEST_F(StepTest, processLabelStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(getLabelStep)
    EXPECT_EQ(out, "Added label to Task with ID " + c.id().value().to_string() + ".\n");
}
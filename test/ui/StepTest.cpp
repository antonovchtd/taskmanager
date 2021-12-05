//
// Created by Anton O. on 12/4/21.
//

#include "../../src/ui/Step.h"
#include "../../src/controller/Action.h"
#include "../../src/ui/Context.h"
#include "../../src/ui/Factory.h"
#include <cstdio>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define STEPTEST_GEN_HOMESTEP_CALL(command, act, step) \
    Context c;                                         \
    std::istringstream is{#command};                   \
    std::ostringstream os;                             \
    c.setOStream(os);                                  \
    c.setIStream(is);                                  \
    HomeStep hs;                                       \
    Factory f;                                         \
    auto action = hs.execute(c, f);                    \
    EXPECT_EQ(action, f.act());                        \
    EXPECT_EQ(c.getStep(), f.step());                  \
    EXPECT_EQ(c.getOldStep(), nullptr);

#define STEPTEST_GEN_WIZARD_STEP_CALL(cls, act) \
    cls stepCls; \
    Context c; \
    Factory f; \
    std::ostringstream os; \
    c.setOStream(os); \
    std::istringstream is{"\n\n\ntest\n10\n2\n\n\n31/12/2022 15:00"}; \
    c.setIStream(is); \
    auto action = stepCls.execute(c, f); \
    EXPECT_EQ(c.data().title, "test"); \
    EXPECT_EQ(c.data().priority, Task::Priority::MEDIUM); \
    time_t rawtime; \
    time(&rawtime); \
    struct tm * timeinfo = localtime(&rawtime); \
    timeinfo->tm_hour = 15; \
    timeinfo->tm_min = 0; \
    timeinfo->tm_sec = 0; \
    timeinfo->tm_mon = 11; \
    timeinfo->tm_mday = 31; \
    timeinfo->tm_year = 122; \
    EXPECT_EQ(c.data().due_date, mktime(timeinfo)); \
    EXPECT_EQ(action, f.act()); \
    EXPECT_EQ(c.getStep(), f.getHomeStep());

#define STEPTEST_GEN_PROCESS_CALL_ACK_ID(cls) \
    cls stepCls; \
    Context c; \
    Factory f; \
    std::ostringstream os; \
    c.setOStream(os); \
    c.setID(TaskID(42)); \
    stepCls.process(c,f); \
    EXPECT_FALSE(os.str().empty());

#define STEPTEST_GEN_PROCESS_DO_NOTHING(cls) \
    cls stepCls; \
    Context c; \
    Factory f; \
    std::ostringstream os; \
    c.setOStream(os); \
    stepCls.process(c, f); \
    EXPECT_TRUE(os.str().empty()); \

class StepTest : public ::testing::Test
{

};

class MockStep : public Step {
public:
    MockStep() = default;
    MOCK_METHOD(std::shared_ptr<Action>, execute, (Context &c, Factory &f), (override));
    MOCK_METHOD(void, process, (Context &c, Factory &f), (override));
    MOCK_METHOD(std::shared_ptr<Action>, getValidateArgAction, (Factory &f), (override));
};

TEST_F(StepTest, shouldPrintMessage)
{
    std::string line = "Hello World!";
    std::ostringstream os;
    MockStep::print(os, line);
    EXPECT_EQ(line, os.str());
}

TEST_F(StepTest, shouldPrintFromFile)
{
    std::string line = "Hello World!\n";
    std::ofstream file;
    std::string fname = "test.txt";
    file.open(fname);
    file << line;
    file.close();
    std::ostringstream os;
    MockStep::printFromFile(os, fname);
    remove(fname.c_str());
    EXPECT_EQ(line, os.str());
}

TEST_F(StepTest, shouldReadInput)
{
    std::string line = "Anton";
    std::istringstream is{line};
    std::ostringstream os;
    std::string answer = MockStep::read(is, os, "Your name: ");
    EXPECT_EQ(line, answer);
}

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
    STEPTEST_GEN_HOMESTEP_CALL(delete, getValidateIDAction, getDeleteStep)
}

TEST_F(StepTest, executeHomeStepWithLabelCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(label, getValidateIDAction, getLabelStep)
}

TEST_F(StepTest, executeHomeStepWithBadCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(bad, getValidateNoArgAction, getHomeStep)
}

TEST_F(StepTest, executeHomeStepWithEmptyCommand)
{
    STEPTEST_GEN_HOMESTEP_CALL(, getValidateNoArgAction, getHomeStep)
}

TEST_F(StepTest, processHomeStepWithMissingIDPrintsMessage)
{
    HomeStep hs;
    Context c;
    Factory f;
    c.setID(std::nullopt);
    std::ostringstream os;
    c.setOStream(os);
    hs.process(c, f);
    EXPECT_FALSE(os.str().empty());
}

TEST_F(StepTest, processHomeStepWithInvalidIDPrintsMessage)
{
    HomeStep hs;
    Context c;
    Factory f;
    c.setID(TaskID::invalidID());
    std::ostringstream os;
    c.setOStream(os);
    hs.process(c, f);
    EXPECT_FALSE(os.str().empty());
}

TEST_F(StepTest, processHomeStepWithValidIDPrintsNoMessage)
{
    HomeStep hs;
    Context c;
    Factory f;
    c.setID(TaskID(1));
    std::ostringstream os;
    c.setOStream(os);
    hs.process(c, f);
    EXPECT_TRUE(os.str().empty());
}

TEST_F(StepTest, shouldExecuteHelpStep)
{
    HelpStep hs;
    Context c;
    Factory f;
    std::ostringstream os;
    c.setOStream(os);
    auto action = hs.execute(c, f);
    EXPECT_FALSE(os.str().empty());
    EXPECT_EQ(c.getStep(), f.getHomeStep());
    EXPECT_EQ(action, nullptr);
}

TEST_F(StepTest, shouldRecordOldStep)
{
    STEPTEST_GEN_HOMESTEP_CALL(help, getValidateNoArgAction, getHelpStep)
    action = hs.execute(c, f);
    EXPECT_EQ(c.getStep(), f.getHomeStep());
    EXPECT_EQ(c.getOldStep(), f.getHelpStep());
}

TEST_F(StepTest, processHelpStepPrintNothing)
{
    STEPTEST_GEN_PROCESS_DO_NOTHING(HelpStep)
}

TEST_F(StepTest, executeAddStepWithFutureDate)
{
    AddStep as;
    Context c;
    Factory f;
    std::ostringstream os;
    c.setOStream(os);
    std::istringstream is{"test\n1\n31/12"};
    c.setIStream(is);
    auto action = as.execute(c, f);
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
    EXPECT_EQ(action, f.getAddTaskAction());
    EXPECT_EQ(c.getStep(), f.getHomeStep());
}

TEST_F(StepTest, executeAddStepWithFutureDateAndTime)
{
    AddStep as;
    Context c;
    Factory f;
    std::ostringstream os;
    c.setOStream(os);
    std::istringstream is{"test\n3\n31/12/21 23:45"};
    c.setIStream(is);
    auto action = as.execute(c, f);
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
    EXPECT_EQ(action, f.getAddTaskAction());
    EXPECT_EQ(c.getStep(), f.getHomeStep());
}

TEST_F(StepTest, executeAddStepWithFutureTime)
{
    AddStep as;
    Context c;
    Factory f;
    std::ostringstream os;
    c.setOStream(os);
    std::istringstream is{"test\n2\nin 00:01:00"};
    c.setIStream(is);
    auto action = as.execute(c, f);
    EXPECT_EQ(c.data().title, "test");
    EXPECT_EQ(c.data().priority, Task::Priority::MEDIUM);
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime(&rawtime);
    timeinfo->tm_hour += 1;
    EXPECT_EQ(c.data().due_date, mktime(timeinfo));
    EXPECT_EQ(action, f.getAddTaskAction());
    EXPECT_EQ(c.getStep(), f.getHomeStep());
}

TEST_F(StepTest, executeAddStepWithMissingEntries)
{
    STEPTEST_GEN_WIZARD_STEP_CALL(AddStep, getAddTaskAction)
}

TEST_F(StepTest, processAddStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(AddStep)
    c.setData(Task::Data{"title", Task::Priority::NONE, time(nullptr), false});
    stepCls.process(c,f);
    EXPECT_TRUE(c.data().title.empty());
    EXPECT_EQ(c.id(), TaskID::invalidID());
}

TEST_F(StepTest, getValidateArgActionReadTaskDataStep)
{
    ReadTaskDataStep rtds;
    Factory f;
    auto action = rtds.getValidateArgAction(f);
    EXPECT_EQ(action, f.getValidateNoArgAction());
}

TEST_F(StepTest, processReadTaskDataStepPrintNothing)
{
    STEPTEST_GEN_PROCESS_DO_NOTHING(ReadTaskDataStep)
}

TEST_F(StepTest, executeEditStep)
{
    STEPTEST_GEN_WIZARD_STEP_CALL(EditStep, getEditAction)
}

TEST_F(StepTest, processEditStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(EditStep)
}

TEST_F(StepTest, executeSubtaskStep)
{
    STEPTEST_GEN_WIZARD_STEP_CALL(SubtaskStep, getAddSubtaskAction)
}

TEST_F(StepTest, processSubtaskStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(SubtaskStep)
}

TEST_F(StepTest, processQuitStepPrintNothing)
{
    STEPTEST_GEN_PROCESS_DO_NOTHING(QuitStep)
}

TEST_F(StepTest, executeShowStep)
{
    ShowStep ss;
    Context c;
    Factory f;
    auto action = ss.execute(c, f);
    EXPECT_EQ(c.getStep(), f.getHomeStep());
    EXPECT_EQ(action, f.getShowAction());
}

TEST_F(StepTest, processShowStep)
{
    ShowStep ss;
    Factory f;
    Context c;
    std::ostringstream os;
    c.setOStream(os);
    TaskManager tm;
    tm.Add(Task::Create("test", Task::Priority::MEDIUM, time(nullptr), false));
    c.setTasks(tm.getTasks());
    ss.process(c, f);
    EXPECT_FALSE(os.str().empty());
}

TEST_F(StepTest, executeCompleteStep)
{
    CompleteStep cs;
    Context c;
    Factory f;
    auto action = cs.execute(c, f);
    EXPECT_EQ(c.getStep(), f.getHomeStep());
    EXPECT_EQ(action, f.getCompleteAction());
}

TEST_F(StepTest, processCompleteStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(CompleteStep)
}

TEST_F(StepTest, executeDeleteStep)
{
    DeleteStep ds;
    Context c;
    Factory f;
    auto action = ds.execute(c, f);
    EXPECT_EQ(c.getStep(), f.getHomeStep());
    EXPECT_EQ(action, f.getDeleteAction());
}

TEST_F(StepTest, processDeleteStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(DeleteStep)
}

TEST_F(StepTest, executeLabelStep)
{
    LabelStep ls;
    Context c;
    Factory f;
    std::ostringstream os;
    c.setOStream(os);
    std::string label = "e";
    std::istringstream is{label};
    c.setIStream(is);
    auto action = ls.execute(c, f);
    EXPECT_EQ(c.getStep(), f.getHomeStep());
    EXPECT_EQ(action, f.getLabelAction());
    EXPECT_FALSE(os.str().empty());
    EXPECT_EQ(label, c.label());
}

TEST_F(StepTest, processLabelStep)
{
    STEPTEST_GEN_PROCESS_CALL_ACK_ID(LabelStep)
}
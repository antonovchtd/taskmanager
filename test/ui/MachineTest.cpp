//
// Created by Anton O. on 12/11/21.
//

//
// Created by Anton O. on 12/6/21.
//

#include "ui/Machine.h"
#include "ui/Step.h"
#include "ui/Controller.h"

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

TEST_F(MachineTest, shouldCreate)
{
    Machine m;
}

TEST_F(MachineTest, shouldReturnModel)
{
    auto tm = std::make_shared<TaskManager>();
    auto con = std::make_shared<Controller>(tm);
    auto mr = std::make_shared<MockReader>();
    auto mp = std::make_shared<MockPrinter>();
    auto f = Factory::create(mr, mp, con);
    Machine m{f, Factory::State::HOME};
    EXPECT_EQ(tm, m.model());
}

TEST_F(MachineTest, shouldExecuteRun)
{
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
                             std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read)
        .WillOnce(Return("quit"));
    Machine m{f, Factory::State::HOME};
    m.run();
}

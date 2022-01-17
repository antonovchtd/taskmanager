//
// Created by Anton O. on 12/11/21.
//

//
// Created by Anton O. on 12/6/21.
//

#include "ui/Machine.h"
#include "model/TaskManager.h"

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
    auto tm = std::shared_ptr<ModelInterface>(new TaskManager);
    Machine m{tm};
}

TEST_F(MachineTest, shouldExecuteRun)
{
    auto tm = std::shared_ptr<ModelInterface>(new TaskManager);
    auto f = Factory::create(std::shared_ptr<AbstractReader>(new MockReader),
                             std::shared_ptr<AbstractPrinter>(new MockPrinter));
    EXPECT_CALL(*std::dynamic_pointer_cast<MockReader>(f->reader()), read)
        .WillOnce(Return("quit"));
    Machine m{tm, f, Factory::State::HOME};
    m.run();
}

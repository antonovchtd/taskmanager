//
// Created by Anton O. on 12/11/21.
//

//
// Created by Anton O. on 12/6/21.
//

#include "../../src/ui/Machine.h"
#include "../../src/ui/Step.h"
#include "../../src/ui/Controller.h"

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

TEST_F(MachineTest, test1)
{

}

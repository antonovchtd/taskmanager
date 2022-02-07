//
// Created by Anton Ovcharenko on 04.02.2022.
//

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "logging/Logger.h"

int main(int argc, char** argv) {
    auto logger = Logger();
    logger.init();
    boost::log::core::get()->set_logging_enabled(false);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
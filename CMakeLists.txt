cmake_minimum_required(VERSION 3.19)
project(taskmanager)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})
#get_filename_component(GTEST_LIB_DIR ${GTEST_LIBRARY} DIRECTORY)
#link_directories(${GTEST_LIB_DIR})

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

file(GLOB Model src/model/*)
file(GLOB ModelTest test/model/*)
file(GLOB UI src/ui/*)
file(GLOB UITest test/ui/*)
file(GLOB IO src/io/*)
add_executable(taskmanager src/main.cpp ${Model} ${UI} ${IO})
add_executable(test_taskmanager ${Model} ${UI} ${IO} ${ModelTest} ${UITest})
target_link_libraries(test_taskmanager GTest::GTest GTest::Main GTest::gmock GTest::gmock_main)
target_link_libraries(test_taskmanager ${Boost_LIBRARIES})
# target_link_libraries(taskmanager ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
gtest_discover_tests(test_taskmanager)

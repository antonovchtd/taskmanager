cmake_minimum_required(VERSION 3.20)
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

add_executable(taskmanager src/main.cpp src/model/Task.cpp src/model/Task.h src/model/TaskManager.cpp src/model/TaskManager.h src/model/TaskID.h src/model/TaskID.cpp src/model/IDGenerator.cpp src/model/IDGenerator.h src/model/Node.cpp src/model/Node.h src/model/Prompt.cpp src/model/Prompt.h)
add_executable(test_taskmanager src/model/Task.cpp src/model/Task.h src/model/TaskManager.cpp src/model/TaskManager.h test/model/TaskTest.cpp src/model/TaskID.h src/model/TaskID.cpp src/model/IDGenerator.cpp src/model/IDGenerator.h test/model/TaskManagerTest.cpp src/model/Node.cpp src/model/Node.h src/model/Prompt.cpp src/model/Prompt.h)
target_link_libraries(test_taskmanager GTest::GTest GTest::Main GTest::gmock GTest::gmock_main)
target_link_libraries(test_taskmanager ${Boost_LIBRARIES})
# target_link_libraries(taskmanager ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
gtest_discover_tests(test_taskmanager)

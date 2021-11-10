cmake_minimum_required(VERSION 3.20)
project(taskmanager)

set(CMAKE_CXX_STANDARD 17)

add_executable(taskmanager src/main.cpp src/model/Task.cpp src/model/Task.h src/model/TaskManager.cpp src/model/TaskManager.h)

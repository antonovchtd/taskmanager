cmake_minimum_required(VERSION 3.20)
project(taskmanager)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})
#get_filename_component(GTEST_LIB_DIR ${GTEST_LIBRARY} DIRECTORY)
#link_directories(${GTEST_LIB_DIR})

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
find_program(_PROTOBUF_PROTOC protoc)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE PROTO_SOURCES CONFIGURE_DEPENDS ${taskmanager_SOURCE_DIR}/proto/*.proto)
message("PROTO_SOURCES ${PROTO_SOURCES}")

set(PROTO_DIR ${taskmanager_SOURCE_DIR}/proto)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR})

macro(compile_proto_file filename)
    get_filename_component(dirname ${filename} PATH)
    get_filename_component(basename ${filename} NAME_WE)

    message("macross ${filename}")
    add_custom_command(
            OUTPUT ${GENERATED_DIR}/${basename}.pb.cc ${GENERATED_DIR}/${basename}.grpc.pb.cc
            DEPENDS ${_PROTOBUF_PROTOC} ${PROTO_DIR}/${basename}.proto
            COMMAND ${_PROTOBUF_PROTOC} ${PROTO_DIR}/${basename}.proto
            --proto_path=${GENERATED_DIR}
            --cpp_out=${GENERATED_DIR}
            --grpc_out=generate_mock_code=true:${GENERATED_DIR}
            --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}
            -I ${PROTO_DIR}
            VERBATIM
    )
endmacro(compile_proto_file)

foreach(proto_file ${PROTO_SOURCES})
    message("compiling ${proto_file}")
    compile_proto_file(${proto_file})
    get_filename_component(basename ${proto_file} NAME_WE)
    set(spine_generated_files ${spine_generated_files} ${GENERATED_DIR}/${basename}.pb.cc ${GENERATED_DIR}/${basename}.pb.h ${GENERATED_DIR}/${basename}.grpc.pb.cc)
endforeach(proto_file)

include_directories(${GENERATED_DIR})


SET_SOURCE_FILES_PROPERTIES(${spine_generated_files} PROPERTIES GENERATED TRUE)


set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

include_directories(src/)
file(GLOB_RECURSE UI src/ui/*)
file(GLOB_RECURSE Model src/model/*)
file(GLOB_RECURSE Trans src/transfer/*)
file(GLOB_RECURSE Utils src/utilities/*)
file(GLOB_RECURSE IO src/io/*)
file(GLOB_RECURSE PER src/persistence/*)
file(GLOB_RECURSE TEST test/*)
add_executable(taskmanager src/main.cpp ${Model} ${Trans} ${Utils} ${UI} ${IO} ${PER}
        ${spine_generated_files})
add_executable(server_taskmanager src/server.cpp ${Model} ${Trans} ${Utils} ${PER} ${spine_generated_files})
add_executable(test_taskmanager ${Model} ${Trans} ${Utils} ${UI} ${IO} ${PER} ${TEST}
        ${spine_generated_files})
target_link_libraries(test_taskmanager GTest::GTest GTest::Main GTest::gmock GTest::gmock_main)
target_link_libraries(test_taskmanager ${Boost_LIBRARIES})
target_link_libraries(taskmanager protobuf::libprotobuf gRPC::grpc++ gRPC::grpc++_reflection)
target_link_libraries(server_taskmanager protobuf::libprotobuf gRPC::grpc++ gRPC::grpc++_reflection)
target_link_libraries(test_taskmanager protobuf::libprotobuf gRPC::grpc++ gRPC::grpc++_reflection)
# target_link_libraries(taskmanager ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
gtest_discover_tests(test_taskmanager)

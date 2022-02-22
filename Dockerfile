FROM ubuntu:latest

ENV TZ=Europe/Kiev
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

##
# Install requirements
#
RUN apt-get update && \
    apt-get install -y wget build-essential automake libtool curl make g++ unzip cmake gcc git pkg-config bash autoconf libtool clang

## GTest
WORKDIR /opt
RUN git clone https://github.com/google/googletest.git -b release-1.11.0 && \
    cd googletest && mkdir build && cd build && cmake .. && make && \
    make install

## GRPC
WORKDIR /opt
RUN mkdir grpc || rm -rf grpc
RUN git clone --recurse-submodules -b v1.43.0 https://github.com/grpc/grpc && \
    cd grpc && git submodule update --init && mkdir -p cmake/build && \
    cd cmake/build && cmake ../.. && make && make install

# # Install third-party libraries and then install gRPC
# WORKDIR ${BASE_PATH}/test/distrib/cpp
# # Remote location for script: https://raw.githubusercontent.com/grpc/grpc/${RELEASE_TAG}/test/distrib/cpp/run_distrib_test_cmake.sh
# RUN ./run_distrib_test_cmake.sh

## BOOST
WORKDIR /opt
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz && \
    tar -zxvf boost_1_78_0.tar.gz && \
    cd boost_1_78_0/ && \
    ./bootstrap.sh --prefix=/usr/ && \
    ./b2 install && \
    export DYLD_LIBRARY_PATH=/opt/boost-1.78.0/libs:$DYLD_LIBRARY_PATH

## Protobuf
WORKDIR /opt
RUN git clone https://github.com/protocolbuffers/protobuf.git && cd protobuf && \
    git submodule update --init --recursive && ./autogen.sh && \
    ./configure --prefix=/usr && make -j$(nproc) && make install && ldconfig 

## TaskManager
WORKDIR /opt
RUN git clone https://github.com/antonovchtd/taskmanager.git && \
    mkdir taskmanager/build && cd taskmanager/build && \
    cmake .. && cmake --build . -- -j 2

EXPOSE 50051
EXPOSE 22

## RUN
ENTRYPOINT ["/opt/taskmanager/build/server_taskmanager", " --debug -p 50051"]


//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "model/ModelInterface.h"
#include "model/TaskManager.h"
#include "transport/TaskManagerGRPCService.h"
#include "logging/Logger.h"

int main(int argc, char** argv) {

    auto logger = Logger()
                  .SetFilename("taskmanager_server.log")
                  .PrintToConsole(true)
                  .SetFilterLevel(logging::trivial::debug);

    logger.init();

    std::string server_address("0.0.0.0:50051");
    auto model = std::shared_ptr<ModelInterface>(std::make_shared<TaskManager>());
    TaskManagerGRPCService service{model};

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    LOG_STREAM(info) << "Server listening on " << server_address;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();

    return 0;
}

//
// Created by Anton Ovcharenko on 26.01.2022.
//

#include <boost/program_options.hpp>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "model/ModelInterface.h"
#include "model/TaskManager.h"
#include "transport/TaskManagerGRPCService.h"
#include "logging/Logger.h"

namespace po = boost::program_options;

int main(int argc, char** argv) {

    std::string hostname;
    std::string port;
    po::options_description general_opts("All available options");
    general_opts.add_options()
            ("debug", "enter debug logging mode")
            ("host,h", po::value<std::string>(&hostname)->default_value("0.0.0.0"),
                    "enter debug logging mode")
            ("port,p", po::value<std::string>(&port)->default_value("50051"),
                    "enter debug logging mode");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, general_opts), vm);
    po::notify(vm);

    auto log_level = logging::trivial::info;
    if (vm.count("debug")) {
        log_level = logging::trivial::debug;
    }

    auto logger = Logger()
            .SetFilename("taskmanager_server.log")
            .PrintToConsole(true)
            .SetFilterLevel(log_level);

    logger.init();

    std::string server_address = hostname + ":" + port;
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

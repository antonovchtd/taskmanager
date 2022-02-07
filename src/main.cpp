/*
 * Task:
 * - add <title> [priority] [due date]
 * - edit <id> <title> [priority] [due date]
 * - delete <id>
 * - complete <id>
 * - show (filter) (sort) (subtasks)
 * - label
 * - subtask <id>
 * UI:
 *  > help
 *  > add [Enter]
 *  add Task >
 *      title: ...
 */

#include "ui/Machine.h"
#include "logging/Logger.h"
#include "transport/TaskManagerGRPCClient.h"

int main() {
    auto logger = Logger()
            .SetFilename("taskmanager_client.log")
            .PrintToConsole(false)
            .SetFilterLevel(logging::trivial::info);

    logger.init();

    std::string target_str = "localhost:50051";
    auto model = std::make_shared<TaskManagerGRPCClient>(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

//    src::severity_logger<logging::trivial::severity_level> lg;
    LOG_STREAM(info) << "Configured connection to " << target_str << ".";

    Machine m{model};

    LOG_STREAM(info) << "Running state machine...";
    m.run();
    return 0;
}

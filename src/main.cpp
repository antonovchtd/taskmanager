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

#include <boost/program_options.hpp>

#include "ui/Machine.h"
#include "logging/Logger.h"
#include "transfer/TaskManagerClient.h"

namespace po = boost::program_options;

int main(int argc, char** argv) {

    std::string hostname;
    std::string port;
    po::options_description general_opts("All available options");
    general_opts.add_options()
            ("debug", "enter debug logging mode")
            ("host,h", po::value<std::string>(&hostname)->default_value("localhost"),
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
            .SetFilename("taskmanager_client.log")
            .PrintToConsole(false)
            .SetFilterLevel(log_level);

    logger.init();

    std::string target_str = hostname + ":" + port;
    auto model = std::make_shared<TaskManagerClient>(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

//    src::severity_logger<logging::trivial::severity_level> lg;
    LOG_STREAM(info) << "Configured connection to " << target_str << ".";

    Machine m{model};

    LOG_STREAM(info) << "Running state machine...";
    m.run();
    return 0;
}

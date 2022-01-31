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
#include "transfer/TaskManagerClient.h"

int main() {

    std::string target_str = "localhost:50051";
    auto model = std::make_shared<TaskManagerClient>(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    Machine m{model};
    m.run();
    return 0;
}

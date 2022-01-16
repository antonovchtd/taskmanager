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

int main() {

    auto tm = std::make_shared<TaskManager>();
    Machine m{tm};
    m.run();
    return 0;
}

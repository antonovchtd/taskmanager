#include <iostream>

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

#include "model/Task.h"
#include "model/TaskManager.h"
#include "ui/Machine.h"

int main() {

    Machine m;
    m.run();
    return 0;
}

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

int main() {
    TaskManager tm;
    TaskID id1 = tm.Add(Task::Create("My First Task",Task::Priority::HIGH,"in 01:10:05"));
    TaskID id2 = tm.Add(Task::Create("Second Task","16/11/21 19:00"));
    TaskID id12 = tm.Add(Task::Create("First Task's subtask","in 02:00:00"), id1);

    std::cout << "First call ------\n";
    tm.Show(std::cout);

    tm.Edit(id2, Task::Create("Second Task edited",Task::Priority::MEDIUM, -1));
    std::cout << "Second call ------\n";
    tm.Show(std::cout);

    tm.Complete(id1);
    std::cout << "Third call ------\n";
    tm.Show(std::cout);

    tm.Delete(id1);
    tm.Add(Task::Create("The Third Task",Task::Priority::LOW));
    std::cout << "Fourth call ------\n";
    tm.Show(std::cout);

    return 0;
}

#include <iostream>

/*
 * Task:
 * - create <title> [priority] [due date]
 * - edit <id> <title> [priority] [due date]
 * - delete <id>
 * - complete <id>
 * - show
 */

#include "model/Task.h"
#include "model/TaskManager.h"

int main() {
    TaskManager tm;
    int id1 = tm.Add(Task::Create("My First Task",Task::Priority::HIGH,3600));
    int id2 = tm.Add(Task::Create("Second Task",3600));

    auto currTasks = tm.getTasks();
    std::cout << "First call ------\n";
    currTasks = tm.getTasks();
    std::cout << currTasks;

    tm.Edit(id2, Task::Create("Second Task edited",Task::Priority::MEDIUM, -1));
    std::cout << "Second call ------\n";
    currTasks = tm.getTasks();
    std::cout << currTasks;

    tm.Complete(id1);
    currTasks = tm.getTasks();
    std::cout << "Third call ------\n";
    std::cout << currTasks;

    tm.Delete(id1);
    tm.Add(Task::Create("The Third Task",Task::Priority::LOW));
    currTasks = tm.getTasks();
    std::cout << "Fourth call ------\n";
    std::cout << currTasks;

    return 0;
}

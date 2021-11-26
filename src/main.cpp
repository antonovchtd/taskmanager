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
//#include "model/Prompt.h"
#include "model/Context.h"
#include "model/State.h"
#include "model/StateFactory.h"

int main() {
//    TaskManager tm;
//    Task::Priority p = Task::Priority::NONE;
//    TaskID id1 = tm.Add(Task::Create("My First Task",Task::Priority::HIGH,"in 10:00",false));
//    TaskID id2 = tm.Add(Task::Create("Second Task", p, "30/11/21 19:00", false));
//    TaskID id12 = tm.Add(Task::Create("First Task's subtask", p,"in 02:00:00", false), id1);
//    TaskID id22 = tm.Add(Task::Create("Second Task's subtask", p,"30/11/2021 19:00", false), id2);
//    TaskID id23 = tm.Add(Task::Create("Second Task's subsubtask", p, "30/11/2021 19:00", false), id22);
//
//    std::cout << "------ First call ------\n";
//    tm.Show(std::cout);
//
//    tm.Edit(id2, Task::Create("Second Task edited",Task::Priority::MEDIUM, time(nullptr)-1, false));
//    std::cout << "------ Second call ------\n";
//    tm.Show(std::cout);
//
//    tm.Complete(id1);
//    std::cout << "------ Third call ------\n";
//    tm.Show(std::cout);
//
//    tm.Delete(id12);
//    tm.Delete(id1);
//    tm.Add(Task::Create("The Third Task",static_cast<Task::Priority>(1), "in 00:30", false));
//    std::cout << "------ Fourth call ------\n";
//    tm.Show(std::cout);

//    Prompt prompt;
//    TaskManager tm2;
//    while (!prompt.quit_flag()){
//        prompt.ask(std::cout);
//        prompt.input(std::cout, std::cin);
//        prompt.act(std::cout, std::cin, tm2);
//    }

    Context c;
    StateFactory f;
    c.changeState(f.nextStep());
    while (c.getState()){
        c.execute(f);
    }

    return 0;
}

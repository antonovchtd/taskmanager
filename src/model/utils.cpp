//
// Created by Anton O. on 12/1/21.
//

#include "utils.h"

std::ostream & operator<<(std::ostream &os, const Task& t) {
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
    time_t dd = t.dueDate();
    std::string str_time = std::string(asctime(localtime(&dd)));
    str_time.pop_back();
    os << ", Due: " << str_time;
    if (t.dueDate() < time(nullptr))
        os << " [overdue] ";

    if (t.isComplete()) {
        os << " [completed] ";
    }
    os << std::endl;
    return os;
}

std::ostream & operator<<(std::ostream &os, const TaskID& id) {
    os << id.value();
    return os;
}

std::ostream & operator<<(std::ostream &os, const std::pair<TaskID, std::pair<Task, Node>>& kv) {
    os << kv.first << " — " << kv.second.first;
    return os;
}

void TaskUtilsPrinter::recursivePrint(std::ostream &os, const std::map<TaskID, std::pair<Task, Node>> &tasks,
                                      const std::pair<TaskID, std::pair<Task, Node>> &kv, const std::string &prefix) {
    os << prefix << kv;
    for (const auto &id : kv.second.second.children()) {
        auto ch = std::make_pair(id, tasks.at(id));
        TaskUtilsPrinter::recursivePrint(os, tasks, ch, prefix + "    ");
    }
}

std::ostream & operator<<(std::ostream &os, const std::map<TaskID, std::pair<Task, Node>>& tasks) {
    for (const auto &kv : tasks) {
        if (!kv.second.second.parent().isValid())
            TaskUtilsPrinter::recursivePrint(os, tasks, kv, "");
    }
    return os;
}


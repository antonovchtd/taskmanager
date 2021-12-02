//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASK_H_
#define TASKMANAGER_SRC_MODEL_TASK_H_

#include <string>
#include <ctime>
#include <map>
#include <iostream>

class Task {
public:
    enum class Priority {
        NONE = 0,
        LOW,
        MEDIUM,
        HIGH
    };
    struct Data{
        std::string title;
        Priority priority;
        time_t due_date;
        bool is_complete = false;
    };

public:
    Task();
    static Task Create(const std::string& title, Task::Priority p, time_t due_date, bool complete_flag);
    static Task Create(const Task::Data&);

public:
    time_t dueDate() const;
    std::string title() const;
    Priority priority() const;
    bool isComplete() const;
    Data data() const;

public:
    bool operator==(const Task&) const;

private:
    Task(std::string title, Task::Priority p, time_t due_date, bool complete_flag);

private:
    std::string title_;
    Priority priority_;
    time_t due_date_;
    bool is_complete_;
};


#endif //TASKMANAGER_SRC_MODEL_TASK_H_

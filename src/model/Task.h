//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASK_H_
#define TASKMANAGER_SRC_MODEL_TASK_H_

#include <string>
#include <ctime>
#include <map>
#include <iostream>

//Value Type
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
        bool isComplete = false;
    };
    static Task Create(const std::string& title, Task::Priority p, time_t due_date, bool complete_flag);
    static Task Create(const Task::Data&);

public:
    Task() : Task("Default", Priority::NONE, 0, false) { };
    time_t due_date() const { return due_date_; };
    std::string title() const { return title_; };
    Priority priority() const { return priority_; };
    bool isComplete() const { return isComplete_; };
    Data data() const;
    bool operator==(const Task&) const;

private:
    Task(std::string title, Task::Priority p, time_t due_date, bool complete_flag);

private:
    std::string title_;
    Priority priority_;
    time_t due_date_;
    bool isComplete_ = false;
};


#endif //TASKMANAGER_SRC_MODEL_TASK_H_

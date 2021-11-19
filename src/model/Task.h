//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASK_H_
#define TASKMANAGER_SRC_MODEL_TASK_H_

#include <string>
#include <ctime>
#include <map>

//Value Type
class Task {
public:
    enum class Priority {
        HIGH = 0,
        MEDIUM,
        LOW,
        NONE
    };
    static Task Create(const std::string& title, Task::Priority p, time_t due_date, bool complete_flag);
    static Task Create(const std::string& title, Task::Priority p, const std::string& due_date, bool complete_flag);

public:
    Task() : Task("Default", Priority::NONE, 0) { };
    time_t due_date() const { return due_date_; };
    std::string title() const { return title_; };
    Priority priority() const { return priority_; };
    bool isComplete() const { return isComplete_; };
    bool operator==(const Task&) const;

private:
    Task(std::string title, Task::Priority p, time_t due_date);
    Task(std::string title, Task::Priority p, const std::string &due_date);

private:
    std::string title_;
    Priority priority_;
    time_t due_date_; // Example of strong typing
    bool isComplete_ = false;
};


#endif //TASKMANAGER_SRC_MODEL_TASK_H_

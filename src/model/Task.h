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
    static Task Create(const std::string& title, Task::Priority p, time_t due_date);
    static Task Create(const std::string& title, Task::Priority p);
    static Task Create(const std::string& title, time_t due_date);
    static Task Create(const std::string& title);
    static Task Create(const std::string& title, Task::Priority p, const std::string& due_date);
    static Task Create(const std::string& title, const std::string& due_date);

public:
    time_t due_date() const { return due_date_; };
    std::string title() const { return title_; };
    Priority priority() const { return priority_; };
    bool isComplete() const { return isComplete_; };
    void setComplete(bool);
    Task() : Task("Default", Priority::NONE, 0) { };
    bool operator==(const Task&) const;

private:
    Task(std::string title, Task::Priority p, time_t due_date);
    Task(std::string title, Task::Priority p, const std::string &due_date);
//    Task(const std::string& title, time_t due_date) : title_(title), priority_(Priority::NONE) {
//        due_date_ = time(nullptr) + due_date;
//    };
//    Task(const std::string& title, Task::Priority p) : title_(title), priority_(p), due_date_(time(nullptr)) { };

private:
    std::string title_;
    Priority priority_;
    time_t due_date_; // Example of strong typing
    bool isComplete_ = false;
};


#endif //TASKMANAGER_SRC_MODEL_TASK_H_

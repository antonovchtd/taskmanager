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
    void setComplete(bool);
    friend std::ostream & operator<<(std::ostream &os, std::map<int, Task>& tasks);
    Task() : Task("", Priority::NONE, 0) { };

private:
    Task(const std::string& title, Task::Priority p, time_t due_date) : title_(title), priority_(p) {
        due_date_ = time(nullptr) + due_date;
    };
//    Task(const std::string& title, time_t due_date) : title_(title), priority_(Priority::NONE) {
//        due_date_ = time(nullptr) + due_date;
//    };
//    Task(const std::string& title, Task::Priority p) : title_(title), priority_(p), due_date_(time(nullptr)) { };

    std::string title_;
    Priority priority_;
    time_t due_date_; // Example of strong typing
    bool isComplete_ = false;
};


#endif //TASKMANAGER_SRC_MODEL_TASK_H_

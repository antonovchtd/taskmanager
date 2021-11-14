//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_


#include <cstdlib>
#include <map>

#include "Task.h"

class TaskID{
public:
    explicit TaskID(unsigned int val) : value_(val) { };
    unsigned int value() const { return value_; };
    bool operator==(const TaskID&) const;
    bool operator<(const TaskID&) const;
private:
    unsigned int value_;
};

class IDGenerator{
public:
    IDGenerator() : last_(0) {};
    explicit IDGenerator(unsigned int last) : last_(last) {};
    TaskID genID(){ return TaskID(last_++); }
private:
    unsigned int last_;
};

class TaskManager {
public:
    TaskID Add(Task t);
    void Edit(TaskID, Task t);
    void Complete(TaskID);
    void Delete(TaskID);
    std::map<TaskID, Task> getTasks();
    void Show(std::ostream &os);
    bool Validate(TaskID id) const;
private:
    std::map<TaskID, Task> tasks_;
    IDGenerator gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

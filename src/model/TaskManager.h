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
    unsigned int Add(Task t);
    void Edit(unsigned int, Task t);
    void Complete(unsigned int);
    void Delete(unsigned int);
    std::map<unsigned int, Task> getTasks();
    void Show();
    bool Validate(TaskID id){
        return tasks_.find(id.value()) != tasks_.end();
    }
private:
    std::map<unsigned int, Task> tasks_;
    IDGenerator gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

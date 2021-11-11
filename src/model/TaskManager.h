//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_


#include <cstdlib>
#include <map>

#include "Task.h"

class IDGenerator{};

class TaskID{
public:
    explicit TaskID(unsigned int val) : value_(val) { };
    unsigned int value() const {return value_;};
private:
    unsigned int value_;
};

bool Validate(TaskID){

}



class TaskManager {
public:
    int Add(Task t);
    void Edit(int, Task t);
    void Complete(int);
    void Delete(int);
    std::map<int, Task> getTasks();
    void Show();
private:
    std::map<int, Task> tasks_;
    size_t counter_ = 0;
    // IDGenerator gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

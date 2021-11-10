//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_


#include <cstdlib>
#include <map>

#include "Task.h"

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
    int counter = 0;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

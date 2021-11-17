//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_


#include <cstdlib>
#include <map>

#include "Task.h"
#include "TaskID.h"
#include "IDGenerator.h"

class TaskManager {
public:
    TaskManager() : gen_(std::shared_ptr<IDGenerator>(new IDGenerator)) {};
    TaskManager(std::shared_ptr<IDGenerator> generator) : gen_(generator) { };
    TaskID Add(Task);
    void Edit(TaskID, Task);
    void Complete(TaskID);
    void Delete(TaskID);
    void Show(std::ostream &os) const;

    std::map<TaskID, Task> getTasks() const;
    bool Validate(TaskID id) const;
    size_t size() const;

private:
    std::map<TaskID, Task> tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

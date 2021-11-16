//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_


#include <cstdlib>
#include <map>

#include "Task.h"
#include "TaskID.h"

class TaskManager {
public:
    TaskID Add(Task);
    void Edit(TaskID, Task);
    void Complete(TaskID);
    void Delete(TaskID);
    void Show(std::ostream &os) const;

    std::map<TaskID, Task> getTasks() const;
    bool Validate(TaskID id) const;

private:
    std::map<TaskID, Task> tasks_;
    IDGenerator gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

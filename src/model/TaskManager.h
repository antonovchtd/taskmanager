//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_


#include <iostream>
#include <cstdlib>
#include <map>
#include <utility>

#include "Task.h"
#include "TaskID.h"
#include "IDGenerator.h"
#include "Node.h"

class TaskManager {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);

public:
    std::pair<Task, Node>& operator[](TaskID);
    std::map<TaskID, std::pair<Task, Node>> getTasks() const;
    std::map<TaskID, std::pair<Task, Node>> getTasks(const std::string &label) const;
    std::map<TaskID, std::pair<Task, Node>> getTasks(const TaskID &id);
    size_t size() const;

public:
    TaskID Add(const Task &);
    TaskID AddSubtask(const Task &, const TaskID &);
    void Edit(const TaskID &, Task);
    void Complete(const TaskID &);
    void Delete(const TaskID &id, bool deleteChildren = false);
    bool Validate(const TaskID &id) const;
    void SetLabel(const TaskID &, const std::string &);

private:
    std::map<TaskID, std::pair<Task, Node>> tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

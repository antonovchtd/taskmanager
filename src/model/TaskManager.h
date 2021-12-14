//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include <iostream>
#include <cstdlib>
#include <map>
#include <utility>
#include <fstream>

#include "Task.h"
#include "TaskID.h"
#include "IDGenerator.h"
#include "Node.h"
#include "Task.pb.h"
#include "utils.h"

class TaskManager {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);

public:
    std::pair<ProtoTask::Task, Node>& operator[](ProtoTask::TaskID);
    std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> getTasks() const;
    std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> getTasks(const std::string &label) const;
    std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> getTasks(const ProtoTask::TaskID &id);
    size_t size() const;

public:
    ProtoTask::TaskID Add(const ProtoTask::Task &);
    ProtoTask::TaskID AddSubtask(const ProtoTask::Task &, const ProtoTask::TaskID &);
    void Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t);
    void Complete(const ProtoTask::TaskID &);
    void Delete(const ProtoTask::TaskID &id, bool deleteChildren = false);
    bool Validate(const ProtoTask::TaskID &id) const;
    void SetLabel(const ProtoTask::TaskID &, const std::string &);

public:
    void read(std::istream &is);
    void readFromFile(const std::string &filename);

private:
    std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

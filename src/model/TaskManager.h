//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <utility>
#include <fstream>

#include "IDGenerator.h"
#include "Node.h"
#include "Task.pb.h"
#include "utils.h"
#include "ActionResult.h"
#include "../persistence/Persister.h"

typedef std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> Container;

class TaskManager {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);
    TaskManager(const std::shared_ptr<IDGenerator> &generator,
                const Container &tasks);

public:
    std::pair<ProtoTask::Task, Node>& operator[](const ProtoTask::TaskID &);
    Container getTasks() const;
    Container getTasks(const std::string &label) const;
    Container getTasks(const ProtoTask::TaskID &id);
    std::shared_ptr<IDGenerator> gen() const;
    size_t size() const;

public:
    ActionResult Add(const ProtoTask::Task &);
    ActionResult AddSubtask(const ProtoTask::Task &, const ProtoTask::TaskID &);
    ActionResult Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t);
    ActionResult Complete(const ProtoTask::TaskID &);
    ActionResult Delete(const ProtoTask::TaskID &id, bool deleteChildren = false);
    ActionResult Validate(const ProtoTask::TaskID &id) const;
    ActionResult SetLabel(const ProtoTask::TaskID &, const std::string &);

public:
    std::vector<ProtoTask::TaskEntity> Export();
    void Replace(const std::vector<ProtoTask::TaskEntity> &);

private:
    Container tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

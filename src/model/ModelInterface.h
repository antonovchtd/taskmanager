//
// Created by Anton O. on 1/5/22.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <utility>
#include <fstream>

#include "IDGenerator.h"
#include "Node.h"
#include "Task.pb.h"
#include "utilities/TaskIDUtils.h"
#include "utilities/TaskUtils.h"
#include "utilities/NodeUtils.h"
#include "ActionResult.h"
#include "persistence/Persister.h"

class ModelInterface {
public:
    virtual std::pair<ProtoTask::Task, Node>& operator[](const ProtoTask::TaskID &) = 0;
    virtual std::vector<ProtoTask::TaskEntity> getTasks() const = 0;
    virtual std::vector<ProtoTask::TaskEntity> getTasks(const std::string &label) const = 0;
    virtual std::vector<ProtoTask::TaskEntity> getTasks(const ProtoTask::TaskID &id) const = 0;
    virtual std::shared_ptr<IDGenerator> gen() const = 0;
    virtual size_t size() const = 0;

public:
    virtual ActionResult Add(const ProtoTask::Task &) = 0;
    virtual ActionResult AddSubtask(const ProtoTask::Task &, const ProtoTask::TaskID &) = 0;
    virtual ActionResult Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t) = 0;
    virtual ActionResult SetComplete(const ProtoTask::TaskID &, bool) = 0;
    virtual ActionResult Delete(const ProtoTask::TaskID &id, bool deleteChildren) = 0;
    virtual ActionResult Validate(const ProtoTask::TaskID &id) const = 0;
    virtual ActionResult SetLabel(const ProtoTask::TaskID &, const std::string &) = 0;

public:
    virtual void Replace(const std::vector<ProtoTask::TaskEntity> &) = 0;

public:
    virtual ~ModelInterface() = default;

};

#endif //TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_

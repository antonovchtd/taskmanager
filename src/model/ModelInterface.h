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
#include "utilities/ActionResult.h"
#include "persistence/Persister.h"

class ModelInterface {
public:
    virtual std::pair<Core::Task, Node>& operator[](const Core::TaskID &) = 0;
    virtual std::vector<Core::TaskEntity> getTasks() const = 0;
    virtual std::vector<Core::TaskEntity> getTasks(const std::string &label) const = 0;
    virtual std::vector<Core::TaskEntity> getTasks(const Core::TaskID &id) const = 0;
    virtual std::shared_ptr<IDGenerator> gen() const = 0;
    virtual size_t size() const = 0;

public:
    virtual ActionResult Add(const Core::Task &) = 0;
    virtual ActionResult AddSubtask(const Core::Task &, const Core::TaskID &) = 0;
    virtual ActionResult Edit(const Core::TaskID &id, const Core::Task &t) = 0;
    virtual ActionResult Complete(const Core::TaskID &) = 0;
    virtual ActionResult Uncomplete(const Core::TaskID &) = 0;
    virtual ActionResult Delete(const Core::TaskID &, bool deleteChildren) = 0;
    virtual ActionResult Validate(const Core::TaskID &) const = 0;
    virtual ActionResult AddLabel(const Core::TaskID &, const std::string &label) = 0;

public:
    virtual void Replace(const std::vector<Core::TaskEntity> &) = 0;

public:
    virtual ~ModelInterface() = default;

};

#endif //TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_

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

#include "Core.pb.h"
class IDGenerator;

class ModelInterface {
public:
    virtual std::vector<Core::TaskEntity> getTasks() const = 0;
    virtual std::vector<Core::TaskEntity> getTasks(const std::string &label) const = 0;
    virtual std::vector<Core::TaskEntity> getTaskWithSubtasks(const Core::TaskID &id) const = 0;

public:
    virtual Core::ModelRequestResult Add(const Core::Task &) = 0;
    virtual Core::ModelRequestResult AddSubtask(const Core::Task &, const Core::TaskID &) = 0;
    virtual Core::ModelRequestResult Edit(const Core::TaskID &id, const Core::Task &t) = 0;
    virtual Core::ModelRequestResult Complete(const Core::TaskID &) = 0;
    virtual Core::ModelRequestResult Uncomplete(const Core::TaskID &) = 0;
    virtual Core::ModelRequestResult Delete(const Core::TaskID &, bool deleteChildren) = 0;
    virtual Core::ModelRequestResult IsPresent(const Core::TaskID &) const = 0;
    virtual Core::ModelRequestResult AddLabel(const Core::TaskID &, const std::string &label) = 0;
    virtual Core::ModelRequestResult ClearLabel(const Core::TaskID &, const std::string &label) = 0;
    virtual Core::ModelRequestResult ClearLabels(const Core::TaskID &) = 0;

public:
    virtual void Replace(const std::vector<Core::TaskEntity> &) = 0;

public:
    virtual ~ModelInterface() = default;

};

#endif //TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_

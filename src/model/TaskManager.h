//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include "ModelInterface.h"

typedef std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> Container;

class TaskManager : public ModelInterface {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);
    TaskManager(const std::shared_ptr<IDGenerator> &generator,
                const Container &tasks);

public:
    std::pair<ProtoTask::Task, Node>& operator[](const ProtoTask::TaskID &) override;
    std::vector<ProtoTask::TaskEntity> getTasks() const override;
    std::vector<ProtoTask::TaskEntity> getTasks(const std::string &label) const override;
    std::vector<ProtoTask::TaskEntity> getTasks(const ProtoTask::TaskID &id) const override;
    std::shared_ptr<IDGenerator> gen() const override;
    size_t size() const override;

public:
    ActionResult Add(const ProtoTask::Task &) override;
    ActionResult AddSubtask(const ProtoTask::Task &, const ProtoTask::TaskID &) override;
    ActionResult Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t) override;
    ActionResult SetComplete(const ProtoTask::TaskID &, bool) override;
    ActionResult Delete(const ProtoTask::TaskID &id, bool deleteChildren) override;
    ActionResult Validate(const ProtoTask::TaskID &id) const override;
    ActionResult SetLabel(const ProtoTask::TaskID &, const std::string &) override;

public:
    void Replace(const std::vector<ProtoTask::TaskEntity> &) override;

private:
    Container tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

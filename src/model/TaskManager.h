//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include "TaskManagerInterface.h"

class TaskManager : public TaskManagerInterface {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);
    TaskManager(const std::shared_ptr<IDGenerator> &generator,
                const Container &tasks);

public:
    std::pair<ProtoTask::Task, Node>& operator[](const ProtoTask::TaskID &) override;
    Container getTasks() const override;
    Container getTasks(const std::string &label) const override;
    Container getTasks(const ProtoTask::TaskID &id) override;
    std::shared_ptr<IDGenerator> gen() const override;
    size_t size() const override;

public:
    ActionResult Add(const ProtoTask::Task &) override;
    ActionResult AddSubtask(const ProtoTask::Task &, const ProtoTask::TaskID &) override;
    ActionResult Edit(const ProtoTask::TaskID &id, const ProtoTask::Task &t) override;
    ActionResult Complete(const ProtoTask::TaskID &) override;
    ActionResult Uncomplete(const ProtoTask::TaskID &) override;
    ActionResult Delete(const ProtoTask::TaskID &id, bool deleteChildren = false) override;
    ActionResult Validate(const ProtoTask::TaskID &id) const override;
    ActionResult SetLabel(const ProtoTask::TaskID &, const std::string &) override;

public:
    std::vector<ProtoTask::TaskEntity> Export() override;
    void Replace(const std::vector<ProtoTask::TaskEntity> &) override;

private:
    ActionResult SetComplete(const ProtoTask::TaskID &, bool);

private:
    Container tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

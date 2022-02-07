//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include "utilities/TaskIDUtils.h"
#include "utilities/TaskUtils.h"
#include "utilities/TaskEntityUtils.h"
#include "utilities/ModelRequestResultUtils.h"
#include "utilities/NodeUtils.h"
#include "ModelInterface.h"
#include "IDGenerator.h"
#include "Node.h"

class TaskManager : public ModelInterface {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);

public:
    std::vector<Core::TaskEntity> getTasks() const override;
    std::vector<Core::TaskEntity> getTasks(const std::string &label) const override;
    std::vector<Core::TaskEntity> getTaskWithSubtasks(const Core::TaskID &id) const override;
    std::shared_ptr<IDGenerator> gen() const;
    size_t size() const;

public:
    Core::ModelRequestResult Add(const Core::Task &) override;
    Core::ModelRequestResult AddSubtask(const Core::Task &, const Core::TaskID &) override;
    Core::ModelRequestResult Edit(const Core::TaskID &id, const Core::Task &t) override;
    Core::ModelRequestResult Complete(const Core::TaskID &) override;
    Core::ModelRequestResult Uncomplete(const Core::TaskID &) override;
    Core::ModelRequestResult Delete(const Core::TaskID &id, bool deleteChildren) override;
    Core::ModelRequestResult IsPresent(const Core::TaskID &id) const override;
    Core::ModelRequestResult AddLabel(const Core::TaskID &, const std::string &) override;
    Core::ModelRequestResult ClearLabel(const Core::TaskID &, const std::string &) override;
    Core::ModelRequestResult ClearLabels(const Core::TaskID &) override;

public:
    void Replace(const std::vector<Core::TaskEntity> &) override;

private:
    void AddChild(const Core::TaskID &parent_id, const Core::TaskID &child_id);
    void RemoveChild(const Core::TaskID &parent, const Core::TaskID &child);

private:
    std::optional<Core::Task> GetTask(const Core::TaskID &id) const;
    std::vector<Core::TaskID> ChildrenOf(const Core::TaskID &parent) const;
    std::optional<Core::TaskID> ParentOf(const Core::TaskID &id) const;

private:
    std::map<Core::TaskID, std::pair<Core::Task, Node>> tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

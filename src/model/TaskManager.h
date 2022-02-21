//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include <mutex>

#include "utilities/TaskIDUtils.h"
#include "utilities/TaskUtils.h"
#include "utilities/TaskEntityUtils.h"
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
    ActionResult Add(const Core::Task &) override;
    ActionResult AddSubtask(const Core::Task &, const Core::TaskID &) override;
    ActionResult Edit(const Core::TaskID &id, const Core::Task &t) override;
    ActionResult Complete(const Core::TaskID &) override;
    ActionResult Uncomplete(const Core::TaskID &) override;
    ActionResult Delete(const Core::TaskID &id, bool deleteChildren) override;
    ActionResult IsPresent(const Core::TaskID &id) const override;
    ActionResult AddLabel(const Core::TaskID &, const std::string &) override;
    ActionResult RemoveLabel(const Core::TaskID &, const std::string &) override;
    ActionResult RemoveAllLabels(const Core::TaskID &) override;

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
    mutable std::recursive_mutex mtx_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

//
// Created by Anton Ovcharenko on 08.11.2021.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGER_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

#include "utilities/TaskIDUtils.h"
#include "utilities/TaskUtils.h"
#include "utilities/NodeUtils.h"
#include "ModelInterface.h"
#include "IDGenerator.h"

typedef std::map<Core::TaskID, std::pair<Core::Task, Node>> Container;

class TaskManager : public ModelInterface {
public:
    TaskManager();
    explicit TaskManager(const std::shared_ptr<IDGenerator> &generator);
    TaskManager(const std::shared_ptr<IDGenerator> &generator,
                const Container &tasks);

public:
    std::vector<Core::TaskEntity> getTasks() const override;
    std::vector<Core::TaskEntity> getTasks(const std::string &label) const override;
    std::vector<Core::TaskEntity> getTasks(const Core::TaskID &id) const override;
    std::shared_ptr<IDGenerator> gen() const override;
    size_t size() const override;

public:
    ActionResult Add(const Core::Task &) override;
    ActionResult AddSubtask(const Core::Task &, const Core::TaskID &) override;
    ActionResult Edit(const Core::TaskID &id, const Core::Task &t) override;
    ActionResult Complete(const Core::TaskID &) override;
    ActionResult Uncomplete(const Core::TaskID &) override;
    ActionResult Delete(const Core::TaskID &id, bool deleteChildren) override;
    ActionResult Validate(const Core::TaskID &id) const override;
    ActionResult AddLabel(const Core::TaskID &, const std::string &) override;

public:
    void Replace(const std::vector<Core::TaskEntity> &) override;

private:
    Container tasks_;
    std::shared_ptr<IDGenerator> gen_;
};


#endif //TASKMANAGER_SRC_MODEL_TASKMANAGER_H_

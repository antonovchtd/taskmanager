//
// Created by Anton O. on 11/30/21.
//

#ifndef TASKMANAGER_SRC_UI_ACTION_H_
#define TASKMANAGER_SRC_UI_ACTION_H_

#include "model/TaskManagerInterface.h"
#include "ui/ControllerInterface.h"
#include "persistence/PersisterInterface.h"
#include "persistence/Persister.h"

class Controller : public ControllerInterface {
public:
    Controller();
    explicit Controller(const std::shared_ptr<TaskManagerInterface> &);
    Controller(const std::shared_ptr<TaskManagerInterface> &, const std::shared_ptr<PersisterInterface> &);

public:
    ActionResult ValidateID(Context &) override;
    ActionResult ValidateNoArg(Context &) override;
    ActionResult ValidateLabelOrID(Context &) override;
    ActionResult ValidateAlpha(Context &) override;
    ActionResult AddTask(Context &) override;
    ActionResult EditTask(Context &) override;
    ActionResult AddSubtask(Context &) override;
    ActionResult ShowTasks(Context &) override;
    ActionResult CompleteTask(Context &) override;
    ActionResult UncompleteTask(Context &) override;
    ActionResult DeleteTask(Context &) override;
    ActionResult ReadTaskWithChildren(Context &) override;
    ActionResult LabelTask(Context &) override;
    ActionResult SaveTasks(Context &) override;
    ActionResult LoadTasks(Context &) override;

public:
    void setData(const Controller::Data &data) override;

public:
    std::shared_ptr<TaskManagerInterface> model() const override;
    Controller::Data data() const override;

private:
    std::shared_ptr<TaskManagerInterface> model_;
    Controller::Data data_;
    std::shared_ptr<PersisterInterface> persister_;
};


#endif //TASKMANAGER_SRC_UI_ACTION_H_

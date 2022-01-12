//
// Created by Anton O. on 11/30/21.
//

#ifndef TASKMANAGER_SRC_UI_ACTION_H_
#define TASKMANAGER_SRC_UI_ACTION_H_

#include "model/ModelInterface.h"
#include "ui/ControllerInterface.h"
#include "persistence/Persister.h"
#include "persistence/FilePersistence.h"

class Controller : public ControllerInterface {
public:
    Controller();
    explicit Controller(std::unique_ptr<ModelInterface>);

public:
    ActionResult ValidateID(Context &) override;
    ActionResult ValidateNoArg(Context &) override;
    ActionResult ValidateAlphaOrID(Context &) override;
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
    Controller::Data data() const override;

private:
    std::unique_ptr<ModelInterface> model_;
    Controller::Data data_;
    std::unique_ptr<Persister> persister_;
};


#endif //TASKMANAGER_SRC_UI_ACTION_H_

//
// Created by Anton O. on 11/30/21.
//

#ifndef TASKMANAGER_SRC_UI_ACTION_H_
#define TASKMANAGER_SRC_UI_ACTION_H_

#include "../model/TaskManager.h"

class Context;

class Controller {
public:
    struct Data {
        std::string arg;
    };

public:
    Controller();
    explicit Controller(const std::shared_ptr<TaskManager> &);
    Controller(const std::shared_ptr<TaskManager> &, const Data &);

public:
    ActionResult ValidateID(Context &);
    ActionResult ValidateNoArg(Context &);
    ActionResult ValidateLabelOrID(Context &);
    ActionResult ValidateAlpha(Context &);
    ActionResult AddTask(Context &);
    ActionResult EditTask(Context &);
    ActionResult AddSubtask(Context &);
    ActionResult ShowTasks(Context &);
    ActionResult CompleteTask(Context &);
    ActionResult DeleteTask(Context &);
    ActionResult ConfirmDeleteTask(Context &);
    ActionResult LabelTask(Context &);
    ActionResult SaveTasks(Context &);
    ActionResult LoadTasks(Context &);

public:
    std::shared_ptr<TaskManager> model() const;
    Controller::Data data() const;
    void setData(const Controller::Data &data);

private:
    std::shared_ptr<TaskManager> model_;
    Controller::Data data_;
    std::string filename_ = "data.bin";
};


#endif //TASKMANAGER_SRC_UI_ACTION_H_

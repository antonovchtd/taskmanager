//
// Created by Anton O. on 1/5/22.
//

#ifndef TASKMANAGER_SRC_UI_CONTROLLERINTERFACE_H_
#define TASKMANAGER_SRC_UI_CONTROLLERINTERFACE_H_

class Context;
class ActionResult;
class TaskManagerInterface;

class ControllerInterface {
public:
    struct Data {
        std::string arg;
    };

public:
    virtual ActionResult ValidateID(Context &) = 0;
    virtual ActionResult ValidateNoArg(Context &) = 0;
    virtual ActionResult ValidateLabelOrID(Context &) = 0;
    virtual ActionResult ValidateAlpha(Context &) = 0;
    virtual ActionResult AddTask(Context &) = 0;
    virtual ActionResult EditTask(Context &) = 0;
    virtual ActionResult AddSubtask(Context &) = 0;
    virtual ActionResult ShowTasks(Context &) = 0;
    virtual ActionResult CompleteTask(Context &) = 0;
    virtual ActionResult UncompleteTask(Context &) = 0;
    virtual ActionResult DeleteTask(Context &) = 0;
    virtual ActionResult ReadTaskWithChildren(Context &) = 0;
    virtual ActionResult LabelTask(Context &) = 0;
    virtual ActionResult SaveTasks(Context &) = 0;
    virtual ActionResult LoadTasks(Context &) = 0;

public:
    virtual void setData(const ControllerInterface::Data &data) = 0;

public:
    virtual std::shared_ptr<TaskManagerInterface> model() const = 0;
    virtual ControllerInterface::Data data() const = 0;

public:
    virtual ~ControllerInterface() = default;
};

#endif //TASKMANAGER_SRC_UI_CONTROLLERINTERFACE_H_

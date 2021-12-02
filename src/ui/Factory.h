//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_UI_FACTORY_H_
#define TASKMANAGER_SRC_UI_FACTORY_H_

#include <memory>
#include <string>
#include <map>

#include "Step.h"
#include "../controller/Action.h"

class Factory{
public:
    enum class State{
        HOME,
        HELP,
        ADD,
        READTASK,
        EDIT,
        SUBTASK,
        QUIT,
        SHOW,
        COMPLETE,
        DELETE
    };

    enum class ActionLabel{
        ADDTASK,
        ADDSUBTASK,
        VALIDATEID,
        VALIDATENOID,
        EDIT,
        SHOW,
        COMPLETE,
        DELETE
    };

public:
    Factory();

    std::shared_ptr<Step> create(const std::string &);

    std::shared_ptr<Step> nextStep();
    std::shared_ptr<Step> nextStep(const HelpStep &);
    std::shared_ptr<Step> nextStep(const AddStep &);
    std::shared_ptr<Step> nextStep(const ReadTaskDataStep &);
    std::shared_ptr<Step> nextStep(const EditStep &);
    std::shared_ptr<Step> nextStep(const SubtaskStep &);
    std::shared_ptr<Step> nextStep(const QuitStep &);
    std::shared_ptr<Step> nextStep(const ShowStep &);
    std::shared_ptr<Step> nextStep(const CompleteStep &);
    std::shared_ptr<Step> nextStep(const DeleteStep &);

    std::shared_ptr<Step> getStep(const State &);
    std::shared_ptr<Step> getHomeStep();
    std::shared_ptr<Step> getHelpStep();
    std::shared_ptr<Step> getAddStep();
    std::shared_ptr<Step> getReadTaskDataStep();
    std::shared_ptr<Step> getEditStep();
    std::shared_ptr<Step> getSubtaskStep();
    std::shared_ptr<Step> getQuitStep();
    std::shared_ptr<Step> getShowStep();
    std::shared_ptr<Step> getCompleteStep();
    std::shared_ptr<Step> getDeleteStep();

    std::shared_ptr<Action> getAction(const HelpStep &);
    std::shared_ptr<Action> getAction(const AddStep &);
    std::shared_ptr<Action> getAction(const ReadTaskDataStep &);
    std::shared_ptr<Action> getAction(const EditStep &);
    std::shared_ptr<Action> getAction(const SubtaskStep &);
    std::shared_ptr<Action> getAction(const QuitStep &);
    std::shared_ptr<Action> getAction(const ShowStep &);
    std::shared_ptr<Action> getAction(const CompleteStep &);
    std::shared_ptr<Action> getAction(const DeleteStep &);

    std::shared_ptr<Action> getAddTaskAction();
    std::shared_ptr<Action> getAddSubtaskAction();
    std::shared_ptr<Action> getValidateIDAction();
    std::shared_ptr<Action> getValidateNoIDAction();
    std::shared_ptr<Action> getEditAction();
    std::shared_ptr<Action> getShowAction();
    std::shared_ptr<Action> getCompleteAction();
    std::shared_ptr<Action> getDeleteAction();

private:
    std::map<State, std::shared_ptr<Step>> steps_;
    std::map<ActionLabel, std::shared_ptr<Action>> actions_;
};

#endif //TASKMANAGER_SRC_UI_FACTORY_H_
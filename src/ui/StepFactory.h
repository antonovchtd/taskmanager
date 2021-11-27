//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_UI_STATEFACTORY_H_
#define TASKMANAGER_SRC_UI_STATEFACTORY_H_

#include <memory>
#include <string>
#include <map>

#include "Step.h"

class StepFactory{
public:
    enum class State{
        HOME,
        HELP,
        ADD,
        READTITLE,
        READPRIORITY,
        READDATE,
        QUIT,
        ADDTASK,
        SHOW
    };

public:
    StepFactory();

    std::shared_ptr<Step> create(const std::string &);

    std::shared_ptr<Step> nextStep();
    std::shared_ptr<Step> nextStep(const HelpStep &);
    std::shared_ptr<Step> nextStep(const AddStep &);
    std::shared_ptr<Step> nextStep(const ReadTitleStep &);
    std::shared_ptr<Step> nextStep(const ReadPriorityStep &);
    std::shared_ptr<Step> nextStep(const ReadDueDateStep &);
    std::shared_ptr<Step> nextStep(const QuitStep &);
    std::shared_ptr<Step> nextStep(const AddTaskStep &);
    std::shared_ptr<Step> nextStep(const ShowStep &);

    std::shared_ptr<Step> getHomeStep();
    std::shared_ptr<Step> getHelpStep();
    std::shared_ptr<Step> getAddStep();
    std::shared_ptr<Step> getReadTitleStep();
    std::shared_ptr<Step> getReadPriorityStep();
    std::shared_ptr<Step> getReadDueDateStep();
    std::shared_ptr<Step> getQuitStep();
    std::shared_ptr<Step> getAddTaskStep();
    std::shared_ptr<Step> getShowStep();
    std::shared_ptr<Step> getStep(const State &);

private:
    std::map<State, std::shared_ptr<Step>> steps_;
};

#endif //TASKMANAGER_SRC_UI_STATEFACTORY_H_

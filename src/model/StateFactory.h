//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_MODEL_STATEFACTORY_H_
#define TASKMANAGER_SRC_MODEL_STATEFACTORY_H_

#include <memory>
#include <string>
#include <map>

#include "State.h"

class StateFactory{
public:
    StateFactory();

    std::shared_ptr<State> create(const std::string &);

    std::shared_ptr<State> nextStep();
    std::shared_ptr<State> nextStep(const HelpState &);
    std::shared_ptr<State> nextStep(const AddState &);
    std::shared_ptr<State> nextStep(const ReadTitleState &);
    std::shared_ptr<State> nextStep(const ReadPriorityState &);
    std::shared_ptr<State> nextStep(const ReadDueDateState &);
    std::shared_ptr<State> nextStep(const QuitState &);
    std::shared_ptr<State> nextStep(const AddTaskState &);
    std::shared_ptr<State> nextStep(const ShowState &);

    std::shared_ptr<State> getHomeState();
    std::shared_ptr<State> getHelpState();
    std::shared_ptr<State> getAddState();
    std::shared_ptr<State> getReadTitleState();
    std::shared_ptr<State> getReadPriorityState();
    std::shared_ptr<State> getReadDueDateState();
    std::shared_ptr<State> getQuitState();
    std::shared_ptr<State> getAddTaskState();
    std::shared_ptr<State> getShowState();

private:
    std::map<std::string, std::shared_ptr<State>> steps_;
};

#endif //TASKMANAGER_SRC_MODEL_STATEFACTORY_H_

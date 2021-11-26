//
// Created by Anton O. on 11/23/21.
//

#include "State.h"
#include "StateFactory.h"

StateFactory::StateFactory() : steps_{{
    {"HomeState", nullptr},
    {"HelpState", nullptr},
    {"AddState", nullptr},
    {"ReadTitleState", nullptr},
    {"ReadPriorityState", nullptr},
    {"ReadDueDateState", nullptr},
    {"QuitState", nullptr},
    {"AddTaskState", nullptr},
    {"ShowState", nullptr}
}}
{
}

std::shared_ptr<State> StateFactory::create(const std::string &command) {
    if (command == "add") {
        return getAddState();
    }
    else if (command == "help"){
        return getHelpState();
    }
    else if (command == "quit"){
        return getQuitState();
    }
    else if (command == "show"){
        return getShowState();
    }
    else{
        std::cout << "Wrong command. Try again. Type `help` for help.\n";
        return getHomeState();
    }
}

std::shared_ptr<State> StateFactory::nextStep() {
    return getHomeState();
}

std::shared_ptr<State> StateFactory::nextStep(const HelpState &) {
    return getHomeState();
}

std::shared_ptr<State> StateFactory::nextStep(const AddState &) {
    return getReadTitleState();
}

std::shared_ptr<State> StateFactory::nextStep(const ReadTitleState &) {
    return getReadPriorityState();
}

std::shared_ptr<State> StateFactory::nextStep(const ReadPriorityState &) {
    return getReadDueDateState();
}

std::shared_ptr<State> StateFactory::nextStep(const ReadDueDateState &) {
    return getAddTaskState();
}

std::shared_ptr<State> StateFactory::nextStep(const QuitState &) {
    return nullptr;
}

std::shared_ptr<State> StateFactory::nextStep(const AddTaskState &) {
    return getHomeState();
}

std::shared_ptr<State> StateFactory::nextStep(const ShowState &) {
    return getHomeState();
}

std::shared_ptr<State> StateFactory::getHomeState() {
    if (!steps_["HomeState"])
        steps_["HomeState"] = std::shared_ptr<State>{new HomeState};
    return steps_["HomeState"];
}

std::shared_ptr<State> StateFactory::getHelpState() {
    if (!steps_["HelpState"])
        steps_["HelpState"] = std::shared_ptr<State>{new HelpState};
    return steps_["HelpState"];
}

std::shared_ptr<State> StateFactory::getAddState() {
    if (!steps_["AddState"])
        steps_["AddState"] = std::shared_ptr<State>{new AddState};
    return steps_["AddState"];
}

std::shared_ptr<State> StateFactory::getReadTitleState() {
    if (!steps_["ReadTitleState"])
        steps_["ReadTitleState"] = std::shared_ptr<State>{new ReadTitleState};
    return steps_["ReadTitleState"];
}

std::shared_ptr<State> StateFactory::getReadPriorityState() {
    if (!steps_["ReadPriorityState"])
        steps_["ReadPriorityState"] = std::shared_ptr<State>{new ReadPriorityState};
    return steps_["ReadPriorityState"];
}

std::shared_ptr<State> StateFactory::getReadDueDateState() {
    if (!steps_["ReadDueDateState"])
        steps_["ReadDueDateState"] = std::shared_ptr<State>{new ReadDueDateState};
    return steps_["ReadDueDateState"];
}

std::shared_ptr<State> StateFactory::getQuitState() {
    if (!steps_["QuitState"])
        steps_["QuitState"] = std::shared_ptr<State>{new QuitState};
    return steps_["QuitState"];
}

std::shared_ptr<State> StateFactory::getAddTaskState() {
    if (!steps_["AddTaskState"])
        steps_["AddTaskState"] = std::shared_ptr<State>{new AddTaskState};
    return steps_["AddTaskState"];
}

std::shared_ptr<State> StateFactory::getShowState() {
    if (!steps_["ShowState"])
        steps_["ShowState"] = std::shared_ptr<State>{new ShowState};
    return steps_["ShowState"];
}
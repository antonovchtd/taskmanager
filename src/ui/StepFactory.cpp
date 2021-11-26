//
// Created by Anton O. on 11/23/21.
//

#include "Step.h"
#include "StepFactory.h"

StepFactory::StepFactory() : steps_{{
    {State::HOME, nullptr},
    {State::HELP, nullptr},
    {State::ADD, nullptr},
    {State::READTITLE, nullptr},
    {State::READPRIORITY, nullptr},
    {State::READDATE, nullptr},
    {State::QUIT, nullptr},
    {State::ADDTASK, nullptr},
    {State::SHOW, nullptr}
}}
{
}

std::shared_ptr<Step> StepFactory::create(const std::string &command) {
    if (command == "add") {
        return getAddStep();
    }
    else if (command == "help"){
        return getHelpStep();
    }
    else if (command == "quit"){
        return getQuitStep();
    }
    else if (command == "show"){
        return getShowStep();
    }
    else{
        std::cout << "Wrong command. Try again. Type `help` for help.\n";
        return getHomeStep();
    }
}

std::shared_ptr<Step> StepFactory::nextStep() {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const HelpStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const AddStep &) {
    return getReadTitleStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadTitleStep &) {
    return getReadPriorityStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadPriorityStep &) {
    return getReadDueDateStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadDueDateStep &) {
    return getAddTaskStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const QuitStep &) {
    return nullptr;
}

std::shared_ptr<Step> StepFactory::nextStep(const AddTaskStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ShowStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::getHomeStep() {
    if (!steps_[State::HOME])
        steps_[State::HOME] = std::shared_ptr<Step>{new HomeStep};
    return steps_[State::HOME];
}

std::shared_ptr<Step> StepFactory::getHelpStep() {
    if (!steps_[State::HELP])
        steps_[State::HELP] = std::shared_ptr<Step>{new HelpStep};
    return steps_[State::HELP];
}

std::shared_ptr<Step> StepFactory::getAddStep() {
    if (!steps_[State::ADD])
        steps_[State::ADD] = std::shared_ptr<Step>{new AddStep};
    return steps_[State::ADD];
}

std::shared_ptr<Step> StepFactory::getReadTitleStep() {
    if (!steps_[State::READTITLE])
        steps_[State::READTITLE] = std::shared_ptr<Step>{new ReadTitleStep};
    return steps_[State::READTITLE];
}

std::shared_ptr<Step> StepFactory::getReadPriorityStep() {
    if (!steps_[State::READPRIORITY])
        steps_[State::READPRIORITY] = std::shared_ptr<Step>{new ReadPriorityStep};
    return steps_[State::READPRIORITY];
}

std::shared_ptr<Step> StepFactory::getReadDueDateStep() {
    if (!steps_[State::READDATE])
        steps_[State::READDATE] = std::shared_ptr<Step>{new ReadDueDateStep};
    return steps_[State::READDATE];
}

std::shared_ptr<Step> StepFactory::getQuitStep() {
    if (!steps_[State::QUIT])
        steps_[State::QUIT] = std::shared_ptr<Step>{new QuitStep};
    return steps_[State::QUIT];
}

std::shared_ptr<Step> StepFactory::getAddTaskStep() {
    if (!steps_[State::ADDTASK])
        steps_[State::ADDTASK] = std::shared_ptr<Step>{new AddTaskStep};
    return steps_[State::ADDTASK];
}

std::shared_ptr<Step> StepFactory::getShowStep() {
    if (!steps_[State::SHOW])
        steps_[State::SHOW] = std::shared_ptr<Step>{new ShowStep};
    return steps_[State::SHOW];
}
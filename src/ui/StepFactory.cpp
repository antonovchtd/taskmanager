//
// Created by Anton O. on 11/23/21.
//

#include "Step.h"
#include "StepFactory.h"

#define GEN_STEP_GETTER(state, step)                   \
  if (!steps_[state])                                  \
      steps_[state] = std::shared_ptr<Step>{new step}; \
  return steps_[state];

StepFactory::StepFactory() : steps_{{
    {State::HOME, nullptr},
    {State::HELP, nullptr},
    {State::ADD, nullptr},
    {State::READID, nullptr},
    {State::READTITLE, nullptr},
    {State::READPRIORITY, nullptr},
    {State::READDATE, nullptr},
    {State::EDIT, nullptr},
    {State::SUBTASK, nullptr},
    {State::ACKNOWLEDGE, nullptr},
    {State::QUIT, nullptr},
    {State::ADDTASK, nullptr},
    {State::ADDSUBTASK, nullptr},
    {State::EDITTASK, nullptr},
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
    else if (command == "edit"){
        return getEditStep();
    }
    else if (command == "subtask"){
        return getSubtaskStep();
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
    return getAddTaskStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadIDStep &) {
    return getReadTitleStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadTitleStep &) {
    return getReadPriorityStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadPriorityStep &) {
    return getReadDueDateStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ReadDueDateStep &) {
    return getQuitStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const EditStep &) {
    return getEditTaskStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const SubtaskStep &) {
    return getAddSubtaskStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const AcknowledgeStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const QuitStep &) {
    return nullptr;
}

std::shared_ptr<Step> StepFactory::nextStep(const AddTaskStep &) {
    return getAcknowledgeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const AddSubtaskStep &) {
    return getAcknowledgeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const EditTaskStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::nextStep(const ShowStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> StepFactory::getHomeStep() {
    GEN_STEP_GETTER(State::HOME, HomeStep)
}

std::shared_ptr<Step> StepFactory::getHelpStep() {
    GEN_STEP_GETTER(State::HELP, HelpStep)
}

std::shared_ptr<Step> StepFactory::getAddStep() {
    GEN_STEP_GETTER(State::ADD, AddStep)
}

std::shared_ptr<Step> StepFactory::getReadIDStep() {
    GEN_STEP_GETTER(State::READID, ReadIDStep)
}

std::shared_ptr<Step> StepFactory::getReadTitleStep() {
    GEN_STEP_GETTER(State::READTITLE, ReadTitleStep)
}

std::shared_ptr<Step> StepFactory::getReadPriorityStep() {
    GEN_STEP_GETTER(State::READPRIORITY, ReadPriorityStep)
}

std::shared_ptr<Step> StepFactory::getReadDueDateStep() {
    GEN_STEP_GETTER(State::READDATE, ReadDueDateStep)
}

std::shared_ptr<Step> StepFactory::getAcknowledgeStep() {
    GEN_STEP_GETTER(State::ACKNOWLEDGE, AcknowledgeStep)
}

std::shared_ptr<Step> StepFactory::getQuitStep() {
    GEN_STEP_GETTER(State::QUIT, QuitStep)
}

std::shared_ptr<Step> StepFactory::getEditStep() {
    GEN_STEP_GETTER(State::EDIT, EditStep)
}

std::shared_ptr<Step> StepFactory::getSubtaskStep() {
    GEN_STEP_GETTER(State::SUBTASK, SubtaskStep)
}

std::shared_ptr<Step> StepFactory::getAddTaskStep() {
    GEN_STEP_GETTER(State::ADDTASK, AddTaskStep)
}

std::shared_ptr<Step> StepFactory::getAddSubtaskStep() {
    GEN_STEP_GETTER(State::ADDSUBTASK, AddSubtaskStep)
}

std::shared_ptr<Step> StepFactory::getEditTaskStep() {
    GEN_STEP_GETTER(State::EDITTASK, EditTaskStep)
}

std::shared_ptr<Step> StepFactory::getShowStep() {
    GEN_STEP_GETTER(State::SHOW, ShowStep)
}

std::shared_ptr<Step> StepFactory::getStep(const State &s) {
    switch (s){
        case State::HOME:
            return getHomeStep();
        case State::HELP:
            return getHelpStep();
        case State::ADD:
            return getAddStep();
        case State::SUBTASK:
            return getSubtaskStep();
        case State::READID:
            return getReadIDStep();
        case State::READTITLE:
            return getReadTitleStep();
        case State::READPRIORITY:
            return getReadPriorityStep();
        case State::READDATE:
            return getReadDueDateStep();
        case State::EDIT:
            return getEditStep();
        case State::ACKNOWLEDGE:
            return getAcknowledgeStep();
        case State::QUIT:
            return getQuitStep();
        case State::ADDTASK:
            return getAddTaskStep();
        case State::ADDSUBTASK:
            return getAddSubtaskStep();
        case State::EDITTASK:
            return getEditTaskStep();
        case State::SHOW:
            return getShowStep();
    }
}
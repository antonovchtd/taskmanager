//
// Created by Anton O. on 11/23/21.
//

#include "Factory.h"

#define FACTORY_GEN_MAP_GETTER(map, cls, state, step) \
  if (!map[state])                                    \
      map[state] = std::shared_ptr<cls>{new step};    \
  return map[state];

Factory::Factory() :
        reader_(std::shared_ptr<AbstractReader>(new ConsoleReader)),
        printer_(std::shared_ptr<AbstractPrinter>(new ConsolePrinter)) {

}

Factory::Factory(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        reader_(std::move(reader)), printer_(std::move(printer)) {

}

std::shared_ptr<AbstractPrinter> Factory::printer() const {
    return printer_;
}

std::shared_ptr<AbstractReader> Factory::reader() const {
    return reader_;
}

std::shared_ptr<Step> Factory::create(const std::string &command) {
    if (command == "add") {
        return getAddStep();
    } else if (command == "help") {
        return getHelpStep();
    } else if (command == "quit") {
        return getQuitStep();
    } else if (command == "show") {
        return getShowStep();
    } else if (command == "edit") {
        return getEditStep();
    } else if (command == "subtask") {
        return getSubtaskStep();
    } else if (command == "complete") {
        return getCompleteStep();
    } else if (command == "delete") {
        return getDeleteStep();
    } else if (command == "label") {
        return getLabelStep();
    } else {
        if (!command.empty())
            printer()->print("Wrong command. Try again. Type `help` for help.\n");
        return getHomeStep();
    }
}

std::shared_ptr<Step> Factory::nextStep() {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const HelpStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const AddStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const ReadTaskDataStep &) {
    return getQuitStep();
}

std::shared_ptr<Step> Factory::nextStep(const EditStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const SubtaskStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const QuitStep &) {
    return nullptr;
}

std::shared_ptr<Step> Factory::nextStep(const ShowStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const CompleteStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const DeleteStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::nextStep(const LabelStep &) {
    return getHomeStep();
}

std::shared_ptr<Step> Factory::getHomeStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::HOME, HomeStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getHelpStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::HELP, HelpStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getAddStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::ADD, AddStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getReadTaskDataStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::READTASK, ReadTaskDataStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getQuitStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::QUIT, QuitStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getEditStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::EDIT, EditStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getSubtaskStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::SUBTASK, SubtaskStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getShowStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::SHOW, ShowStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getCompleteStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::COMPLETE, CompleteStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getDeleteStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::DELETE, DeleteStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getLabelStep() {
    FACTORY_GEN_MAP_GETTER(steps_, Step, State::LABEL, LabelStep(reader_, printer_))
}

std::shared_ptr<Step> Factory::getStep(const State &s) {
    switch (s){
        case State::HOME:
            return getHomeStep();
        case State::HELP:
            return getHelpStep();
        case State::ADD:
            return getAddStep();
        case State::SUBTASK:
            return getSubtaskStep();
        case State::READTASK:
            return getReadTaskDataStep();
        case State::EDIT:
            return getEditStep();
        case State::QUIT:
            return getQuitStep();
        case State::SHOW:
            return getShowStep();
        case State::COMPLETE:
            return getCompleteStep();
        case State::DELETE:
            return getDeleteStep();
        case State::LABEL:
            return getLabelStep();
    }
}

std::shared_ptr<Action> Factory::getAction(const HelpStep &) {
    return nullptr;
}

std::shared_ptr<Action> Factory::getAction(const AddStep &) {
    return getAddTaskAction();
}

std::shared_ptr<Action> Factory::getAction(const ReadTaskDataStep &) {
    return nullptr;
}

std::shared_ptr<Action> Factory::getAction(const EditStep &) {
    return getEditAction();
}

std::shared_ptr<Action> Factory::getAction(const SubtaskStep &) {
    return getAddSubtaskAction();
}

std::shared_ptr<Action> Factory::getAction(const QuitStep &) {
    return nullptr;
}

std::shared_ptr<Action> Factory::getAction(const ShowStep &) {
    return getShowAction();
}

std::shared_ptr<Action> Factory::getAction(const CompleteStep &) {
    return getCompleteAction();
}

std::shared_ptr<Action> Factory::getAction(const DeleteStep &) {
    return getDeleteAction();
}

std::shared_ptr<Action> Factory::getAction(const LabelStep &) {
    return getLabelAction();
}

std::shared_ptr<Action> Factory::getAddTaskAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action, ActionLabel::ADDTASK, AddTaskAction)
}

std::shared_ptr<Action> Factory::getAddSubtaskAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::ADDSUBTASK,
                           AddSubtaskAction)
}

std::shared_ptr<Action> Factory::getValidateIDAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::VALIDATEID,
                           ValidateIDAction)
}

std::shared_ptr<Action> Factory::getValidateNoArgAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::VALIDATENOID,
                           ValidateNoArgAction)
}

std::shared_ptr<Action> Factory::getValidateLabelArgAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::VALIDATELABEL,
                           ValidateLabelArgAction)
}

std::shared_ptr<Action> Factory::getEditAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::EDIT,
                           EditTaskAction)
}

std::shared_ptr<Action> Factory::getShowAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::SHOW,
                           ShowAction)
}

std::shared_ptr<Action> Factory::getCompleteAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::COMPLETE,
                           CompleteTaskAction)
}

std::shared_ptr<Action> Factory::getDeleteAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::DELETE,
                           DeleteTaskAction)
}

std::shared_ptr<Action> Factory::getLabelAction(){
    FACTORY_GEN_MAP_GETTER(actions_, Action,
                           ActionLabel::LABEL,
                           LabelAction)
}
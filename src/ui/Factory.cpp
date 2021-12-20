//
// Created by Anton O. on 11/23/21.
//

#include "Factory.h"
#include "Step.h"

Factory::Factory() :
        reader_(std::shared_ptr<AbstractReader>(new ConsoleReader)),
        printer_(std::shared_ptr<AbstractPrinter>(new ConsolePrinter)),
        model_(std::shared_ptr<TaskManager>(new TaskManager)) {

}

Factory::Factory(const std::shared_ptr<AbstractReader> &reader,
                 const std::shared_ptr<AbstractPrinter> &printer) :
         reader_(reader), printer_(printer),
         model_(std::shared_ptr<TaskManager>(new TaskManager)) {

}

Factory::Factory(const std::shared_ptr<AbstractReader> &reader,
                 const std::shared_ptr<AbstractPrinter> &printer,
                 const std::shared_ptr<TaskManager> &model) :
         Factory(reader, printer) {
    model_ = model;
}

std::shared_ptr<Factory> Factory::create() {
    return std::shared_ptr<Factory>(new Factory);
}

std::shared_ptr<Factory> Factory::create(const std::shared_ptr<AbstractReader> &reader,
                                         const std::shared_ptr<AbstractPrinter> &printer) {
    return std::shared_ptr<Factory>(new Factory(reader, printer));
}

std::shared_ptr<AbstractPrinter> Factory::printer() const {
    return printer_;
}

std::shared_ptr<AbstractReader> Factory::reader() const {
    return reader_;
}

std::shared_ptr<TaskManager> Factory::model() const {
    return model_;
}

std::shared_ptr<Step> Factory::createStep(const std::string &command) {
    if (command == "add") {
        return lazyInitStep(Factory::State::ADD);
    } else if (command == "help") {
        return lazyInitStep(Factory::State::HELP);
    } else if (command == "quit") {
        return lazyInitStep(Factory::State::QUIT);
    } else if (command == "show") {
        return lazyInitStep(Factory::State::SHOW);
    } else if (command == "edit") {
        return lazyInitStep(Factory::State::EDIT);
    } else if (command == "subtask") {
        return lazyInitStep(Factory::State::SUBTASK);
    } else if (command == "complete") {
        return lazyInitStep(Factory::State::COMPLETE);
    } else if (command == "delete") {
        return lazyInitStep(Factory::State::CONFIRMDELETE);
    } else if (command == "label") {
        return lazyInitStep(Factory::State::LABEL);
    } else if (command == "save") {
        return lazyInitStep(Factory::State::SAVE);
    } else if (command == "load") {
        return lazyInitStep(Factory::State::LOAD);
    } else {
        if (!command.empty())
            printer()->print("Wrong command. Try again. Type `help` for help.\n");
        return lazyInitStep(Factory::State::HOME);
    }
}

std::shared_ptr<Step> Factory::getNewStep(const State &s) {
    switch (s) {
        case State::HOME:
            return std::shared_ptr<HomeStep>{new HomeStep(shared_from_this())};
        case State::HELP:
            return std::shared_ptr<HelpStep>{new HelpStep(shared_from_this())};
        case State::ADD:
            return std::shared_ptr<AddStep>{new AddStep(shared_from_this())};
        case State::SUBTASK:
            return std::shared_ptr<SubtaskStep>{new SubtaskStep(shared_from_this())};
        case State::READTASK:
            return std::shared_ptr<ReadTaskDataStep>{new ReadTaskDataStep(shared_from_this())};
        case State::EDIT:
            return std::shared_ptr<EditStep>{new EditStep(shared_from_this())};
        case State::QUIT:
            return std::shared_ptr<QuitStep>{new QuitStep(shared_from_this())};
        case State::SHOW:
            return std::shared_ptr<ShowStep>{new ShowStep(shared_from_this())};
        case State::COMPLETE:
            return std::shared_ptr<CompleteStep>{new CompleteStep(shared_from_this())};
        case State::DELETE:
            return std::shared_ptr<DeleteStep>{new DeleteStep(shared_from_this())};
        case State::CONFIRMDELETE:
            return std::shared_ptr<ConfirmDeleteStep>{new ConfirmDeleteStep(shared_from_this())};
        case State::LABEL:
            return std::shared_ptr<LabelStep>{new LabelStep(shared_from_this())};
        case State::SAVE:
            return std::shared_ptr<SaveStep>{new SaveStep(shared_from_this())};
        case State::LOAD:
            return std::shared_ptr<LoadStep>{new LoadStep(shared_from_this())};
    }
}

std::shared_ptr<Step> Factory::lazyInitStep(const Factory::State &state) {
    if (steps_.find(state) == steps_.end())
        steps_[state] = getNewStep(state);
    return steps_[state];
}

std::shared_ptr<Action> Factory::lazyInitAction(const Factory::ActionLabel &label) {
    if (actions_.find(label) == actions_.end())
        actions_[label] = getNewAction(label);
    return actions_[label];
}

std::shared_ptr<Action> Factory::getNewAction(const Factory::ActionLabel &label) {
    switch (label){
        case Factory::ActionLabel::DONOTHING:
            return std::make_shared<DoNothingAction>(model_);
        case Factory::ActionLabel::ADDTASK:
            return std::make_shared<AddTaskAction>(model_);
        case Factory::ActionLabel::ADDSUBTASK:
            return std::make_shared<AddSubtaskAction>(model_);
        case Factory::ActionLabel::VALIDATEID:
            return std::make_shared<GetIDAction>(model_);
        case Factory::ActionLabel::VALIDATENOID:
            return std::make_shared<ValidateNoArgAction>(model_);
        case Factory::ActionLabel::VALIDATELABEL:
            return std::make_shared<ValidateLabelOrIDArgAction>(model_);
        case Factory::ActionLabel::EDIT:
            return std::make_shared<EditTaskAction>(model_);
        case Factory::ActionLabel::SHOW:
            return std::make_shared<ShowAction>(model_);
        case Factory::ActionLabel::COMPLETE:
            return std::make_shared<CompleteTaskAction>(model_);
        case Factory::ActionLabel::DELETE:
            return std::make_shared<DeleteAction>(model_);
        case Factory::ActionLabel::CONFIRMDELETE:
            return std::make_shared<ConfirmDeleteAction>(model_);
        case Factory::ActionLabel::LABEL:
            return std::make_shared<LabelAction>(model_);
        case Factory::ActionLabel::SAVE:
            return std::make_shared<SaveAction>(model_);
        case Factory::ActionLabel::LOAD:
            return std::make_shared<LoadAction>(model_);
    }
}

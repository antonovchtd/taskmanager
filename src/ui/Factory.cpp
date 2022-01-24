//
// Created by Anton O. on 11/23/21.
//

#include "Factory.h"
#include "ui/steps/Step.h"
#include "ui/steps/HomeStep.h"
#include "ui/steps/HelpStep.h"
#include "ui/steps/AddStep.h"
#include "ui/steps/ReadTaskDataStep.h"
#include "ui/steps/SubtaskStep.h"
#include "ui/steps/EditStep.h"
#include "ui/steps/QuitStep.h"
#include "ui/steps/ShowStep.h"
#include "ui/steps/CompleteStep.h"
#include "ui/steps/UncompleteStep.h"
#include "ui/steps/DeleteStep.h"
#include "ui/steps/ConfirmDeleteStep.h"
#include "ui/steps/LabelStep.h"
#include "Machine.h"

Factory::Factory() :
        reader_(std::shared_ptr<AbstractReader>(new ConsoleReader)),
        printer_(std::shared_ptr<AbstractPrinter>(new ConsolePrinter)) {

}

Factory::Factory(const std::shared_ptr<AbstractReader> &reader,
                 const std::shared_ptr<AbstractPrinter> &printer) :
         reader_(reader), printer_(printer) {

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
    } else if (command == "uncomplete") {
        return lazyInitStep(Factory::State::UNCOMPLETE);
    } else if (command == "delete") {
        return lazyInitStep(Factory::State::CONFIRMDELETE);
    } else if (command == "label") {
        return lazyInitStep(Factory::State::LABEL);
    } else if (command == "save") {
        return lazyInitStep(Factory::State::HOME);
    } else if (command == "load") {
        return lazyInitStep(Factory::State::HOME);
    } else {
        if (!command.empty())
            printer()->print("Wrong command. Try again. Type `help` for help.\n");
        return lazyInitStep(Factory::State::HOME);
    }
}

std::shared_ptr<Machine> Factory::createMachine(const State &state) {
    return std::make_shared<Machine>(std::shared_ptr<ModelInterface>(new TaskManager), shared_from_this(), state);
}

std::shared_ptr<Step> Factory::getNewStep(const State &s) {
    switch (s) {
        case State::HOME:
            return std::shared_ptr<HomeStep>{new HomeStep(reader(), printer())};
        case State::HELP:
            return std::shared_ptr<HelpStep>{new HelpStep(reader(), printer())};
        case State::ADD:
            return std::shared_ptr<AddStep>{new AddStep(printer(), createMachine(State::READTASK))};
        case State::SUBTASK:
            return std::shared_ptr<SubtaskStep>{new SubtaskStep(printer(), createMachine(State::READTASK))};
        case State::READTASK:
            return std::shared_ptr<ReadTaskDataStep>{new ReadTaskDataStep(reader(), printer())};
        case State::EDIT:
            return std::shared_ptr<EditStep>{new EditStep(printer(), createMachine(State::READTASK))};
        case State::QUIT:
            return std::shared_ptr<QuitStep>{new QuitStep(printer())};
        case State::SHOW:
            return std::shared_ptr<ShowStep>{new ShowStep(printer())};
        case State::COMPLETE:
            return std::shared_ptr<CompleteStep>{new CompleteStep(printer())};
        case State::UNCOMPLETE:
            return std::shared_ptr<UncompleteStep>{new UncompleteStep(printer())};
        case State::DELETE:
            return std::shared_ptr<DeleteStep>{new DeleteStep(printer())};
        case State::CONFIRMDELETE:
            return std::shared_ptr<ConfirmDeleteStep>{new ConfirmDeleteStep(reader(), printer())};
        case State::LABEL:
            return std::shared_ptr<LabelStep>{new LabelStep(reader(), printer())};
    }
}

std::shared_ptr<Step> Factory::lazyInitStep(const Factory::State &state) {
    if (steps_.find(state) == steps_.end())
        steps_[state] = getNewStep(state);
    return steps_[state];
}


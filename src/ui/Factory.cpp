//
// Created by Anton O. on 11/23/21.
//

#include "Factory.h"
#include "Step.h"
#include "Machine.h"

Factory::Factory() :
        reader_(std::shared_ptr<AbstractReader>(new ConsoleReader)),
        printer_(std::shared_ptr<AbstractPrinter>(new ConsolePrinter)),
        controller_(std::shared_ptr<ControllerInterface>(new Controller))  {

}

Factory::Factory(const std::shared_ptr<AbstractReader> &reader,
                 const std::shared_ptr<AbstractPrinter> &printer) :
         reader_(reader), printer_(printer),
         controller_(std::shared_ptr<ControllerInterface>(new Controller)) {

}

Factory::Factory(const std::shared_ptr<AbstractReader> &reader,
        const std::shared_ptr<AbstractPrinter> &printer,
        const std::shared_ptr<ControllerInterface> &controller) :
        reader_(reader), printer_(printer), controller_(controller) {

}

std::shared_ptr<Factory> Factory::create() {
    return std::shared_ptr<Factory>(new Factory);
}

std::shared_ptr<Factory> Factory::create(const std::shared_ptr<AbstractReader> &reader,
                                         const std::shared_ptr<AbstractPrinter> &printer) {
    return std::shared_ptr<Factory>(new Factory(reader, printer));
}

std::shared_ptr<Factory> Factory::create(const std::shared_ptr<AbstractReader> &reader,
                                         const std::shared_ptr<AbstractPrinter> &printer,
                                         const std::shared_ptr<ControllerInterface> &controller) {
    return std::shared_ptr<Factory>(new Factory(reader, printer, controller));
}

std::shared_ptr<AbstractPrinter> Factory::printer() const {
    return printer_;
}

std::shared_ptr<AbstractReader> Factory::reader() const {
    return reader_;
}

std::shared_ptr<ControllerInterface> Factory::controller() const {
    return controller_;
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
        return lazyInitStep(Factory::State::SAVE);
    } else if (command == "load") {
        return lazyInitStep(Factory::State::LOAD);
    } else {
        if (!command.empty())
            printer()->print("Wrong command. Try again. Type `help` for help.\n");
        return lazyInitStep(Factory::State::HOME);
    }
}

Machine Factory::createMachine(const State &state) {
    return {shared_from_this(), state};
}

Machine Factory::createMachine(const State &state, const Context &context) {
    return {shared_from_this(), state, context};
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
        case State::UNCOMPLETE:
            return std::shared_ptr<UncompleteStep>{new UncompleteStep(shared_from_this())};
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


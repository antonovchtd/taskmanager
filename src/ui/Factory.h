//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_UI_FACTORY_H_
#define TASKMANAGER_SRC_UI_FACTORY_H_

#include <memory>
#include <string>
#include <map>

#include "Action.h"
#include "../model/TaskManager.h"
#include "../io/AbstractReader.h"
#include "../io/ConsoleReader.h"
#include "../io/FileReader.h"
#include "../io/AbstractPrinter.h"
#include "../io/ConsolePrinter.h"
#include "../io/FilePrinter.h"

class Step;

class Factory : public std::enable_shared_from_this<Factory> {
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
        DELETE,
        CONFIRMDELETE,
        LABEL
    };

    enum class ActionLabel{
        DONOTHING,
        ADDTASK,
        ADDSUBTASK,
        VALIDATEID,
        VALIDATENOID,
        VALIDATELABEL,
        EDIT,
        SHOW,
        COMPLETE,
        DELETE,
        CONFIRMDELETE,
        LABEL
    };

public:
    static std::shared_ptr<Factory> create();
    static std::shared_ptr<Factory> create(const std::shared_ptr<AbstractReader> &, const std::shared_ptr<AbstractPrinter> &);

private:
    Factory();
    Factory(const std::shared_ptr<AbstractReader> &,
            const std::shared_ptr<AbstractPrinter> &);
    Factory(const std::shared_ptr<AbstractReader> &,
            const std::shared_ptr<AbstractPrinter> &,
            const std::shared_ptr<TaskManager> &);

public:
    std::shared_ptr<Step> createStep(const std::string &command);

    std::shared_ptr<Step> getNewStep(const State &s);
    std::shared_ptr<Step> lazyInitStep(const Factory::State &state);

    std::shared_ptr<Action> getNewAction(const Factory::ActionLabel &);
    std::shared_ptr<Action> lazyInitAction(const Factory::ActionLabel &label);

public:
    std::shared_ptr<AbstractReader> reader() const;
    std::shared_ptr<AbstractPrinter> printer() const;
    std::shared_ptr<TaskManager> model() const;

private:
    std::map<State, std::shared_ptr<Step>> steps_;
    std::map<ActionLabel, std::shared_ptr<Action>> actions_;
    std::shared_ptr<AbstractReader> reader_;
    std::shared_ptr<AbstractPrinter> printer_;
    std::shared_ptr<TaskManager> model_;
};

#endif //TASKMANAGER_SRC_UI_FACTORY_H_

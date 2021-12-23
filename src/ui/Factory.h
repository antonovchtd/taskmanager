//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_UI_FACTORY_H_
#define TASKMANAGER_SRC_UI_FACTORY_H_

#include <memory>
#include <string>
#include <map>

#include "Controller.h"
#include "model/TaskManager.h"
#include "io/AbstractReader.h"
#include "io/ConsoleReader.h"
#include "io/FileReader.h"
#include "io/AbstractPrinter.h"
#include "io/ConsolePrinter.h"
#include "io/FilePrinter.h"

class Step;
class Machine;

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
        UNCOMPLETE,
        DELETE,
        CONFIRMDELETE,
        LABEL,
        SAVE,
        LOAD
    };

public:
    static std::shared_ptr<Factory> create();
    static std::shared_ptr<Factory> create(const std::shared_ptr<AbstractReader> &, const std::shared_ptr<AbstractPrinter> &);

private:
    Factory();
    Factory(const std::shared_ptr<AbstractReader> &,
            const std::shared_ptr<AbstractPrinter> &);

public:
    std::shared_ptr<Step> createStep(const std::string &command);

    std::shared_ptr<Step> getNewStep(const State &s);
    std::shared_ptr<Step> lazyInitStep(const State &state);

    Machine createMachine(const State &state);

public:
    std::shared_ptr<AbstractReader> reader() const;
    std::shared_ptr<AbstractPrinter> printer() const;
    std::shared_ptr<Controller> controller() const;

private:
    std::map<State, std::shared_ptr<Step>> steps_;
    std::shared_ptr<AbstractReader> reader_;
    std::shared_ptr<AbstractPrinter> printer_;
    std::shared_ptr<Controller> controller_;
};

#endif //TASKMANAGER_SRC_UI_FACTORY_H_

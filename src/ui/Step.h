//
// Created by Anton O. on 11/22/21.
//

#ifndef TASKMANAGER_SRC_UI_STATE_H_
#define TASKMANAGER_SRC_UI_STATE_H_

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <regex>
#include <optional>
#include <sstream>

#include "../model/Task.h"
#include "../model/TaskID.h"
#include "../model/Node.h"
#include "../io/AbstractReader.h"
#include "../io/FileReader.h"
#include "../io/AbstractPrinter.h"

class Context;
class Action;
class Factory;

class Step{
public:
    Step(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);

public:
    virtual std::shared_ptr<Action> execute(Context &c, Factory &f) = 0;
    virtual void process(Context &c, Factory &f) = 0;
    virtual std::shared_ptr<Action> getValidateArgAction(Factory &f) = 0;
    virtual ~Step() = default;

public:
    std::shared_ptr<AbstractReader> reader() const;
    std::shared_ptr<AbstractPrinter> printer() const;

private:
    std::shared_ptr<AbstractReader> reader_;
    std::shared_ptr<AbstractPrinter> printer_;
};

class HomeStep : public Step {
public:
    HomeStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class HelpStep : public Step {
public:
    HelpStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class AddStep : public Step {
public:
    AddStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class ReadTaskDataStep : public Step {
public:
    ReadTaskDataStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;

public:
    std::optional<time_t> stringToTime(const Context &c, const std::string &datestring);
    bool validateTitle(const Context &c, const std::string &title);
    std::optional<Task::Priority> stringToPriority(const Context &c, const std::string &priority);
};

class EditStep : public Step {
public:
    EditStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class SubtaskStep : public Step {
public:
    SubtaskStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class QuitStep : public Step {
public:
    QuitStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class ShowStep : public Step {
public:
    ShowStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
    void recursivePrint(const std::pair<TaskID, std::pair<Task, Node>> &kv,
                        const Context &c,
                        const std::string &prefix);
};

class CompleteStep : public Step {
public:
    CompleteStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class DeleteStep : public Step {
public:
    DeleteStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class ConfirmDeleteStep : public Step {
public:
    ConfirmDeleteStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

class LabelStep : public Step {
public:
    LabelStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateArgAction(Factory &f) override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

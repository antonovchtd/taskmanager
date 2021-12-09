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
    Step(const std::shared_ptr<AbstractReader> &reader,
         const std::shared_ptr<AbstractPrinter> &printer,
         const std::shared_ptr<Factory> &factory);

public:
    virtual std::shared_ptr<Action> execute(Context &c) = 0;
    virtual void process(Context &c) = 0;
    virtual std::shared_ptr<Action> getValidateArgAction() = 0;
    virtual ~Step() = default;

public:
    std::shared_ptr<AbstractReader> reader() const;
    std::shared_ptr<AbstractPrinter> printer() const;
    std::shared_ptr<Factory> factory() const;

private:
    std::shared_ptr<AbstractReader> reader_;
    std::shared_ptr<AbstractPrinter> printer_;
    std::shared_ptr<Factory> factory_;
};

class HomeStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class HelpStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class AddStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class ReadTaskDataStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;

public:
    std::optional<time_t> stringToTime(const Context &c, const std::string &datestring);
    bool validateTitle(const Context &c, const std::string &title);
    std::optional<Task::Priority> stringToPriority(const Context &c, const std::string &priority);
};

class EditStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class SubtaskStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class QuitStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class ShowStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
    void recursivePrint(const std::pair<TaskID, std::pair<Task, Node>> &kv,
                        const Context &c,
                        const std::string &prefix);
};

class CompleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class DeleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class ConfirmDeleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

class LabelStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Action> execute(Context &c) override;
    void process(Context &c) override;
    std::shared_ptr<Action> getValidateArgAction() override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

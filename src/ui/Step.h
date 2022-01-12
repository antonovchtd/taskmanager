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

#include "Task.pb.h"
#include "model/Node.h"
#include "io/AbstractReader.h"
#include "io/AbstractPrinter.h"

class StepSwitcher;
class Context;
class Controller;
class Factory;

class Step{
public:
    virtual std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) = 0;
    virtual ~Step() = default;
};

class HomeStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
    std::string command() const;
private:
    std::string command_;
};

class HelpStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class AddStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class ReadTaskDataStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;

public:
    std::optional<time_t> stringToTime(const std::string &datestring) const;
    std::string timeToString(const time_t &date) const;
    bool validateTitle(const std::string &title) const;
    std::optional<Core::Task::Priority> stringToPriority(const std::string &priority) const;
    std::string priorityToString(const Core::Task::Priority &priority) const;

public:
    void readTitle(Context &, const std::shared_ptr<Factory> &) const;
    void readPriority(Context &, const std::shared_ptr<Factory> &) const;
    void readDueDate(Context &, const std::shared_ptr<Factory> &) const;
};

class EditStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class SubtaskStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class QuitStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class ShowStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
    void recursivePrint(const Core::TaskEntity &te,
                        const std::shared_ptr<Factory> &f,
                        const Context &c,
                        const std::string &prefix);
};

class CompleteStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class UncompleteStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class DeleteStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class ConfirmDeleteStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;

public:
    std::shared_ptr<Step> getConfirmation(Context &c, const std::shared_ptr<Factory> &f);
};

class LabelStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;

public:
    std::string readLabel(const std::shared_ptr<Factory> &f) const;
};

class SaveStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

class LoadStep : public Step {
public:
    std::shared_ptr<Step> execute(Context &c, const std::shared_ptr<Factory> &f) override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

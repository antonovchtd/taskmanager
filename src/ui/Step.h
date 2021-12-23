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

#include "model/Task.pb.h"
#include "model/Node.h"
#include "io/AbstractReader.h"
#include "io/FileReader.h"
#include "io/AbstractPrinter.h"

class StepSwitcher;
class Context;
class Controller;
class Factory;

class Step{
public:
    explicit Step(const std::shared_ptr<Factory> &factory);

public:
    virtual std::shared_ptr<Step> execute(Context &c) = 0;
    virtual ~Step() = default;

public:
    std::shared_ptr<Factory> factory() const;

private:
    std::shared_ptr<Factory> factory_;
};

class HomeStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
    std::string command() const;
private:
    std::string command_;
};

class HelpStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class AddStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class ReadTaskDataStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;

public:
    std::optional<time_t> stringToTime(const Context &c, const std::string &datestring);
    bool validateTitle(const Context &c, const std::string &title);
    std::optional<ProtoTask::Task::Priority> stringToPriority(const Context &c, const std::string &priority);
};

class EditStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class SubtaskStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class QuitStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class ShowStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
    void recursivePrint(const std::pair<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> &kv,
                        const Context &c,
                        const std::string &prefix);
};

class CompleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class UncompleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class DeleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class ConfirmDeleteStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class LabelStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class SaveStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

class LoadStep : public Step {
public:
    using Step::Step;
    std::shared_ptr<Step> execute(Context &c) override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

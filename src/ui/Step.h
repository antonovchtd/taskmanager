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

class Context;
class Action;
class Factory;

class Step{
public:
    virtual std::shared_ptr<Action> execute(Context &c, Factory &f) = 0;
    virtual void process(Context &c, Factory &f) = 0;
    virtual std::shared_ptr<Action> getValidateIDAction(Factory &f) = 0;
    virtual ~Step() = default;

public:
    static std::string read(const std::string &prompt);
    static void print(const std::string &line);
    static void printFromFile(const std::string &fname);
};

class HomeStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class HelpStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class AddStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class ReadTaskDataStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;

public:
    static std::optional<time_t> stringToTime(const std::string &);
    static bool validateTitle(const std::string &);
    static std::optional<Task::Priority> stringToPriority(const std::string &);
};

class EditStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class SubtaskStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class QuitStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class ShowStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class CompleteStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class DeleteStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

class LabelStep : public Step{
public:
    std::shared_ptr<Action> execute(Context &c, Factory &f) override;
    void process(Context &c, Factory &f) override;
    std::shared_ptr<Action> getValidateIDAction(Factory &f) override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

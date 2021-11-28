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

//#include "StepFactory.h"
class StepFactory;

class Context;

class Step{
public:
    virtual void execute(Context &c, StepFactory &f) = 0;
    static std::string read(const std::string &prompt);
    static void print(const std::string &line);
    virtual ~Step() = default;

};

class HomeStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class HelpStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class AddStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class ReadIDStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class ReadTitleStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class ReadPriorityStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class ReadDueDateStep : public Step{
public:
    static std::optional<time_t> stringToTime(const std::string &);
    void execute(Context &c, StepFactory &f) override;
};

class EditStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class SubtaskStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class AcknowledgeStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class QuitStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class AddTaskStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class EditTaskStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class AddSubtaskStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class ShowStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class CompleteStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

class DeleteStep : public Step{
public:
    void execute(Context &c, StepFactory &f) override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

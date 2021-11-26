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

public:
    void changeStep(const std::shared_ptr<Context> &, const std::shared_ptr<Step> &);
};

class HomeStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

class HelpStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

class AddStep : public Step{
    void execute(Context &c, StepFactory &f) override;

};

class ReadTitleStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

class ReadPriorityStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

class ReadDueDateStep : public Step{
    static std::optional<time_t> stringToTime(const std::string &);
    void execute(Context &c, StepFactory &f) override;
};

class QuitStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

class AddTaskStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

class ShowStep : public Step{
    void execute(Context &c, StepFactory &f) override;
};

#endif //TASKMANAGER_SRC_UI_STATE_H_

//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_READTASKDATASTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_READTASKDATASTEP_H_

#include <regex>

#include "IOStep.h"
#include "utilities/StepUtils.h"
#include "ui/actions/DoNothingAction.h"

class ReadTaskDataStep : public IOStep {
public:
    ReadTaskDataStep(const std::shared_ptr<AbstractReader> &reader,
                     const std::shared_ptr<AbstractPrinter> &printer);
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

public:
    void readTitle(Context &) const;
    void readPriority(Context &) const;
    void readDueDate(Context &) const;

public:
    std::optional<time_t> stringToTime(const std::string &datestring) const;
    std::string timeToString(const time_t &date) const;
    bool validateTitle(const std::string &title) const;
    std::optional<Core::Task::Priority> stringToPriority(const std::string &priority) const;
    std::string priorityToString(const Core::Task::Priority &priority) const;
};


#endif //TASKMANAGER_SRC_UI_STEPS_READTASKDATASTEP_H_

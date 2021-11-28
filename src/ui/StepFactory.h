//
// Created by Anton O. on 11/23/21.
//

#ifndef TASKMANAGER_SRC_UI_STATEFACTORY_H_
#define TASKMANAGER_SRC_UI_STATEFACTORY_H_

#include <memory>
#include <string>
#include <map>

#include "Step.h"

class StepFactory{
public:
    enum class State{
        HOME,
        HELP,
        ADD,
        READID,
        READTITLE,
        READPRIORITY,
        READDATE,
        EDIT,
        SUBTASK,
        ACKNOWLEDGE,
        QUIT,
        ADDTASK,
        EDITTASK,
        ADDSUBTASK,
        SHOW,
        COMPLETE,
        DELETE
    };

public:
    StepFactory();

    std::shared_ptr<Step> create(const std::string &);

    std::shared_ptr<Step> nextStep();
    std::shared_ptr<Step> nextStep(const HelpStep &);
    std::shared_ptr<Step> nextStep(const AddStep &);
    std::shared_ptr<Step> nextStep(const ReadIDStep &);
    std::shared_ptr<Step> nextStep(const ReadTitleStep &);
    std::shared_ptr<Step> nextStep(const ReadPriorityStep &);
    std::shared_ptr<Step> nextStep(const ReadDueDateStep &);
    std::shared_ptr<Step> nextStep(const EditStep &);
    std::shared_ptr<Step> nextStep(const SubtaskStep &);
    std::shared_ptr<Step> nextStep(const AcknowledgeStep &);
    std::shared_ptr<Step> nextStep(const QuitStep &);
    std::shared_ptr<Step> nextStep(const AddTaskStep &);
    std::shared_ptr<Step> nextStep(const EditTaskStep &);
    std::shared_ptr<Step> nextStep(const AddSubtaskStep &);
    std::shared_ptr<Step> nextStep(const ShowStep &);
    std::shared_ptr<Step> nextStep(const CompleteStep &);
    std::shared_ptr<Step> nextStep(const DeleteStep &);

    std::shared_ptr<Step> getHomeStep();
    std::shared_ptr<Step> getHelpStep();
    std::shared_ptr<Step> getAddStep();
    std::shared_ptr<Step> getReadIDStep();
    std::shared_ptr<Step> getReadTitleStep();
    std::shared_ptr<Step> getReadPriorityStep();
    std::shared_ptr<Step> getReadDueDateStep();
    std::shared_ptr<Step> getEditStep();
    std::shared_ptr<Step> getSubtaskStep();
    std::shared_ptr<Step> getAcknowledgeStep();
    std::shared_ptr<Step> getQuitStep();
    std::shared_ptr<Step> getAddTaskStep();
    std::shared_ptr<Step> getEditTaskStep();
    std::shared_ptr<Step> getAddSubtaskStep();
    std::shared_ptr<Step> getShowStep();
    std::shared_ptr<Step> getCompleteStep();
    std::shared_ptr<Step> getDeleteStep();
    std::shared_ptr<Step> getStep(const State &);

private:
    std::map<State, std::shared_ptr<Step>> steps_;
};

#endif //TASKMANAGER_SRC_UI_STATEFACTORY_H_

//
// Created by Anton Ovcharenko on 10.12.2021.
//

#ifndef TASKMANAGER_SRC_UI_STEPSWITCHER_H_
#define TASKMANAGER_SRC_UI_STEPSWITCHER_H_

#include "Step.h"
#include "Factory.h"

class StepSwitcher {
public:
    static std::shared_ptr<Step> nextStep(const HomeStep &);
    static std::shared_ptr<Step> nextStep(const HelpStep &);
    static std::shared_ptr<Step> nextStep(const AddStep &);
    static std::shared_ptr<Step> nextStep(const ReadTaskDataStep &);
    static std::shared_ptr<Step> nextStep(const SubtaskStep &);
    static std::shared_ptr<Step> nextStep(const EditStep &);
    static std::shared_ptr<Step> nextStep(const QuitStep &);
    static std::shared_ptr<Step> nextStep(const ShowStep &);
    static std::shared_ptr<Step> nextStep(const CompleteStep &);
    static std::shared_ptr<Step> nextStep(const DeleteStep &);
    static std::shared_ptr<Step> nextStep(const ConfirmDeleteStep &);
    static std::shared_ptr<Step> nextStep(const LabelStep &);
    static std::shared_ptr<Step> nextStep(const SaveStep &);
    static std::shared_ptr<Step> nextStep(const LoadStep &);
};

#endif //TASKMANAGER_SRC_UI_STEPSWITCHER_H_

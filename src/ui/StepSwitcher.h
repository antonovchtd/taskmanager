//
// Created by Anton Ovcharenko on 10.12.2021.
//

#ifndef TASKMANAGER_SRC_UI_STEPSWITCHER_H_
#define TASKMANAGER_SRC_UI_STEPSWITCHER_H_

#include <memory>

class Factory;
class Step;
class HomeStep;
class HelpStep;
class AddStep;
class ReadTaskDataStep;
class SubtaskStep;
class EditStep;
class QuitStep;
class ShowStep;
class CompleteStep;
class UncompleteStep;
class DeleteStep;
class DeleteStep;
class ConfirmDeleteStep;
class LabelStep;

class StepSwitcher {
public:
    static std::shared_ptr<Step> nextStep(const HomeStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const HelpStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const AddStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const ReadTaskDataStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const SubtaskStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const EditStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const QuitStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const ShowStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const CompleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const UncompleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const DeleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const ConfirmDeleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Step> nextStep(const LabelStep &, const std::shared_ptr<Factory> &);
};

#endif //TASKMANAGER_SRC_UI_STEPSWITCHER_H_

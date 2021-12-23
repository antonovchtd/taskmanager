//
// Created by Anton Ovcharenko on 10.12.2021.
//

#include "StepSwitcher.h"

std::shared_ptr<Step> StepSwitcher::nextStep(const HomeStep &step) {
    return step.factory()->createStep(step.command());
}

std::shared_ptr<Step> StepSwitcher::nextStep(const HelpStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const AddStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const ReadTaskDataStep &step) {
    return step.factory()->lazyInitStep(Factory::State::QUIT);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const EditStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const SubtaskStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const QuitStep &step) {
    return nullptr;
}

std::shared_ptr<Step> StepSwitcher::nextStep(const ShowStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const CompleteStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const UncompleteStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const DeleteStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const ConfirmDeleteStep &step) {
    return step.factory()->lazyInitStep(Factory::State::DELETE);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const LabelStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const SaveStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const LoadStep &step) {
    return step.factory()->lazyInitStep(Factory::State::HOME);
}

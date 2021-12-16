//
// Created by Anton Ovcharenko on 10.12.2021.
//

#include "StepSwitcher.h"

std::shared_ptr<Step> StepSwitcher::nextStep(const HelpStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const AddStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const ReadTaskDataStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::QUIT);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const EditStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const SubtaskStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const QuitStep &, const std::shared_ptr<Factory> &factory) {
    return nullptr;
}

std::shared_ptr<Step> StepSwitcher::nextStep(const ShowStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const CompleteStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const DeleteStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const ConfirmDeleteStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::DELETE);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const LabelStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const SaveStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}

std::shared_ptr<Step> StepSwitcher::nextStep(const LoadStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitStep(Factory::State::HOME);
}
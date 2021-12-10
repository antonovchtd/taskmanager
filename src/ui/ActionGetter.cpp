//
// Created by Anton Ovcharenko on 10.12.2021.
//

#include "ActionGetter.h"

std::shared_ptr<Action> ActionGetter::getAction(const HelpStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::DONOTHING);
}

std::shared_ptr<Action> ActionGetter::getAction(const AddStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::ADDTASK);
}

std::shared_ptr<Action> ActionGetter::getAction(const ReadTaskDataStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::DONOTHING);
}

std::shared_ptr<Action> ActionGetter::getAction(const EditStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::EDIT);
}

std::shared_ptr<Action> ActionGetter::getAction(const SubtaskStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::ADDSUBTASK);
}

std::shared_ptr<Action> ActionGetter::getAction(const QuitStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::DONOTHING);
}

std::shared_ptr<Action> ActionGetter::getAction(const ShowStep &, const std::shared_ptr<Factory> &factory) {
    std::shared_ptr<Action> action = factory->lazyInitAction(Factory::ActionLabel::SHOW);
    std::shared_ptr<Action> labelAction = factory->lazyInitAction(Factory::ActionLabel::VALIDATELABEL);
    action->setActionData(labelAction->data());
    return action;
}

std::shared_ptr<Action> ActionGetter::getAction(const CompleteStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::COMPLETE);
}

std::shared_ptr<Action> ActionGetter::getAction(const DeleteStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::DELETE);
}

std::shared_ptr<Action> ActionGetter::getAction(const ConfirmDeleteStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::CONFIRMDELETE);
}

std::shared_ptr<Action> ActionGetter::getAction(const LabelStep &, const std::shared_ptr<Factory> &factory) {
    return factory->lazyInitAction(Factory::ActionLabel::LABEL);
}



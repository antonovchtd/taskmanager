//
// Created by Anton Ovcharenko on 10.12.2021.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONGETTER_H_
#define TASKMANAGER_SRC_UI_ACTIONGETTER_H_

#include "Step.h"
#include "Factory.h"
#include "Action.h"

class ActionGetter {
public:
    static std::shared_ptr<Action> getAction(const HelpStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const AddStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const ReadTaskDataStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const EditStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const SubtaskStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const QuitStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const ShowStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const CompleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const DeleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const ConfirmDeleteStep &, const std::shared_ptr<Factory> &);
    static std::shared_ptr<Action> getAction(const LabelStep &, const std::shared_ptr<Factory> &);
};

#endif //TASKMANAGER_SRC_UI_ACTIONGETTER_H_

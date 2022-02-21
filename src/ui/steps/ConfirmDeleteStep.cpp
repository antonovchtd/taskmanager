//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ConfirmDeleteStep.h"
#include "ui/Context.h"

ConfirmDeleteStep::ConfirmDeleteStep(const std::shared_ptr<AbstractReader> &reader,
                                     const std::shared_ptr<AbstractPrinter> &printer) :
                                     IOStep(reader, printer) {
}

std::unique_ptr<Action> ConfirmDeleteStep::genAction(Context &) {
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> ConfirmDeleteStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return getConfirmation(factory);
}

std::shared_ptr<Step> ConfirmDeleteStep::getConfirmation(const std::shared_ptr<Factory> &factory) {
    while (true) {
        std::string reply = reader()->read("Delete task with all its subtasks? Y/[N] > ");
        if (reply == "Y" || reply == "y") {
            Core::ModelRequestResult result;
            result.set_status(Core::ModelRequestResult_Status_SUCCESS);
            return processResult(*this, factory, result, "");
        } else if (reply.empty() || reply == "N" || reply == "n") {
            // disregard nextStep and go to HomeStep
            return factory->lazyInitStep(Factory::State::HOME);
        } else {
            printer()->print("Wrong option. Type Y or N.\n");
        }
    }
}
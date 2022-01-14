//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UTILITIES_STEPUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_STEPUTILS_H_

#include "ui/steps/Step.h"
#include "ui/StepSwitcher.h"
#include "ui/Factory.h"

template <typename T>
std::shared_ptr<Step> processResult(const T &step,
                                    const std::shared_ptr<Factory> &factory,
                                    const ActionResult &result,
                                    const std::string &message) {
    if (result) {
        if (!message.empty()) {
            step.printer()->print(message);
            if (result.id)
                step.printer()->print(" (ID: " + std::to_string(result.id->value()) + ")\n");
        }
    }
    else {
        step.printer()->print(result.message());
        return factory->lazyInitStep(Factory::State::HOME);
    }
    return StepSwitcher::nextStep(step, factory);
}

#endif //TASKMANAGER_SRC_UTILITIES_STEPUTILS_H_

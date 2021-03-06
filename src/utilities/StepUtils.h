//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UTILITIES_STEPUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_STEPUTILS_H_

#include "ui/steps/Step.h"
#include "ui/StepSwitcher.h"
#include "ui/Factory.h"
#include "utilities/ModelRequestResultUtils.h"


template <typename T>
std::shared_ptr<Step> processResult(const T &step,
                                    const std::shared_ptr<Factory> &factory,
                                    const ActionResult &result,
                                    const std::string &message) {
    if (result.model_result && ToBool(*result.model_result)) {
        if (!message.empty()) {
            step.printer()->print(message);
            if (result.model_result->has_id())
                step.printer()->print(" (ID: " + std::to_string(result.model_result->id().value()) + ")\n");
        }
    } else if (result.entity || result.tasks) {
        // move on
    }
    else {
        if (result.model_result && result.model_result->has_status())
            step.printer()->print(ToString(result.model_result->status()));
        return factory->lazyInitStep(Factory::State::HOME);
    }
    return StepSwitcher::nextStep(step, factory);
}

#endif //TASKMANAGER_SRC_UTILITIES_STEPUTILS_H_

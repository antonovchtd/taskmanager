//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_HOMESTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_HOMESTEP_H_

#include <sstream>

#include "Step.h"
#include "utilities/StepUtils.h"
#include "ui/actions/ValidateIDAction.h"
#include "ui/actions/ValidateNoArgAction.h"
#include "ui/actions/ValidateAlphaOrIDAction.h"
#include "ui/actions/GetTasksToShowAction.h"
#include "ui/actions/LoadFromFileAction.h"
#include "ui/actions/SaveToFileAction.h"

class HomeStep : public Step {
public:
    using Step::Step;
    std::unique_ptr<Action> genAction(Context &) override;
    std::shared_ptr<Step> genNextStep(const ActionResult &, const std::shared_ptr<Factory> &) override;

public:
    std::string command() const;

private:
    std::string command_;
};


#endif //TASKMANAGER_SRC_UI_STEPS_HOMESTEP_H_

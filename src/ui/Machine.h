//
// Created by Anton O. on 11/26/21.
//

#ifndef TASKMANAGER_SRC_UI_MACHINE_H_
#define TASKMANAGER_SRC_UI_MACHINE_H_

#include "model/ModelInterface.h"
#include "model/TaskManager.h"
#include "Context.h"
#include "ui/steps/Step.h"
#include "ui/actions/Action.h"
#include "ui/Factory.h"

class Machine {
public:
    Machine();
    Machine(const std::shared_ptr<Factory> &f, const Factory::State &s);

public:
    Context run();
    Context run(const Context &context);

private:
    Context context_;
    std::shared_ptr<Factory> factory_;
    Factory::State initial_step_;
    std::shared_ptr<ModelInterface> model_;
};


#endif //TASKMANAGER_SRC_UI_MACHINE_H_

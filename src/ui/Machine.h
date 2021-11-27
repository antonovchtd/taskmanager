//
// Created by Anton O. on 11/26/21.
//

#ifndef TASKMANAGER_SRC_UI_MACHINE_H_
#define TASKMANAGER_SRC_UI_MACHINE_H_

#include "Context.h"
#include "StepFactory.h"

class Machine {
public:
    Context run(std::optional<StepFactory::State> state = std::nullopt);
};


#endif //TASKMANAGER_SRC_UI_MACHINE_H_

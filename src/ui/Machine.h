//
// Created by Anton O. on 11/26/21.
//

#ifndef TASKMANAGER_SRC_UI_MACHINE_H_
#define TASKMANAGER_SRC_UI_MACHINE_H_

#include "Context.h"
#include "StepFactory.h"

class Machine {
public:
    Machine() = default;
    explicit Machine(const Context &c) : context_(c) {};
    Context run(std::optional<StepFactory::State> state = std::nullopt);

private:
    Context context_;
    StepFactory factory_;
};


#endif //TASKMANAGER_SRC_UI_MACHINE_H_

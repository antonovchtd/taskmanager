//
// Created by Anton O. on 11/26/21.
//

#ifndef TASKMANAGER_SRC_UI_MACHINE_H_
#define TASKMANAGER_SRC_UI_MACHINE_H_

#include "model/TaskManagerInterface.h"
#include "Context.h"
#include "Factory.h"

class Machine {
public:
    Machine();
    Machine(const std::shared_ptr<Factory> &f, const Factory::State &s);

public:
    Context run();

public:
    std::shared_ptr<TaskManagerInterface> model() const;

private:
    Context context_;
    std::shared_ptr<Factory> factory_;
    Factory::State initial_step_;
};


#endif //TASKMANAGER_SRC_UI_MACHINE_H_

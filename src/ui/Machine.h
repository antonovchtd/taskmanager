//
// Created by Anton O. on 11/26/21.
//

#ifndef TASKMANAGER_SRC_UI_MACHINE_H_
#define TASKMANAGER_SRC_UI_MACHINE_H_

#include "../model/TaskManager.h"
#include "../controller/Controller.h"
#include "Context.h"
#include "Factory.h"

class Machine {
public:
    Machine();
    explicit Machine(const Factory::State &s);
    Machine(Context c, const Factory::State &s);

public:
    Context run();

public:
    TaskManager model() const;

private:
    Context context_;
    Factory factory_;
    Controller controller_;
    TaskManager model_;
};


#endif //TASKMANAGER_SRC_UI_MACHINE_H_

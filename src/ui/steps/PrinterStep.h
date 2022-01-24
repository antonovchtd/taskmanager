//
// Created by Anton Ovcharenko on 24.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_PRINTERSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_PRINTERSTEP_H_

#include "Step.h"
#include "io/AbstractPrinter.h"

class PrinterStep : public Step {
public:
    PrinterStep(const std::shared_ptr<AbstractPrinter> &printer);

public:
    ~PrinterStep() override = default;

public:
    std::shared_ptr<AbstractPrinter> printer() const;

private:
    std::shared_ptr<AbstractPrinter> printer_;
};

#endif //TASKMANAGER_SRC_UI_STEPS_PRINTERSTEP_H_

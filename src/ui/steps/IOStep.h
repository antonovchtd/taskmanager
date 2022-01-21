//
// Created by Anton Ovcharenko on 21.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_STEPS_IOSTEP_H_
#define TASKMANAGER_SRC_UI_STEPS_IOSTEP_H_

#include "Step.h"
#include "io/AbstractReader.h"
#include "io/AbstractPrinter.h"

class IOStep : public Step {
public:
    IOStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer);

public:
    ~IOStep() override = default;

public:
    std::shared_ptr<AbstractReader> reader() const;
    std::shared_ptr<AbstractPrinter> printer() const;

private:
    std::shared_ptr<AbstractReader> reader_;
    std::shared_ptr<AbstractPrinter> printer_;
};

#endif //TASKMANAGER_SRC_UI_STEPS_IOSTEP_H_

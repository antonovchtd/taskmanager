//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "Step.h"

Step::Step(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        reader_{reader}, printer_{printer} {
}

std::shared_ptr<AbstractReader> Step::reader() const {
    return reader_;
}

std::shared_ptr<AbstractPrinter> Step::printer() const {
    return printer_;
}
//
// Created by Anton Ovcharenko on 21.01.2022.
//

#include "IOStep.h"

IOStep::IOStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        reader_{reader}, printer_{printer} {
}

std::shared_ptr<AbstractReader> IOStep::reader() const {
    return reader_;
}

std::shared_ptr<AbstractPrinter> IOStep::printer() const {
    return printer_;
}
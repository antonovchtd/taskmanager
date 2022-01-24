//
// Created by Anton Ovcharenko on 24.01.2022.
//

#include "PrinterStep.h"

PrinterStep::PrinterStep(const std::shared_ptr<AbstractPrinter> &printer) : printer_{printer} {
}

std::shared_ptr<AbstractPrinter> PrinterStep::printer() const {
    return printer_;
}
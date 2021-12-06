//
// Created by Anton O. on 12/6/21.
//

#ifndef TASKMANAGER_SRC_IO_CONSOLEPRINTER_H_
#define TASKMANAGER_SRC_IO_CONSOLEPRINTER_H_

#include <iostream>

#include "AbstractPrinter.h"

class ConsolePrinter : public AbstractPrinter {
public:
    void print(const std::string &message) override;
};


#endif //TASKMANAGER_SRC_IO_CONSOLEPRINTER_H_

//
// Created by Anton O. on 12/6/21.
//

#ifndef TASKMANAGER_SRC_IO_ABSTRACTPRINTER_H_
#define TASKMANAGER_SRC_IO_ABSTRACTPRINTER_H_

#include <string>

class AbstractPrinter {
public:
    virtual void print(const std::string &message) = 0;
    virtual ~AbstractPrinter() = default;
};


#endif //TASKMANAGER_SRC_IO_ABSTRACTPRINTER_H_

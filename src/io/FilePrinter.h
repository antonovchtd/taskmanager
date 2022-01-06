//
// Created by Anton O. on 12/6/21.
//

#ifndef TASKMANAGER_SRC_IO_FILEPRINTER_H_
#define TASKMANAGER_SRC_IO_FILEPRINTER_H_

#include <fstream>

#include "AbstractPrinter.h"

class FilePrinter : public AbstractPrinter {
public:
    explicit FilePrinter(const std::string &filename);
    void print(const std::string &message) override;

private:
    std::string filename_;
};


#endif //TASKMANAGER_SRC_IO_FILEPRINTER_H_

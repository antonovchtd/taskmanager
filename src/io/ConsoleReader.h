//
// Created by Anton O. on 12/6/21.
//

#ifndef TASKMANAGER_SRC_IO_CONSOLEREADER_H_
#define TASKMANAGER_SRC_IO_CONSOLEREADER_H_

#include <iostream>

#include "AbstractReader.h"

class ConsoleReader : public AbstractReader {
public:
    std::string read(const std::string &prompt) override;
};


#endif //TASKMANAGER_SRC_IO_CONSOLEREADER_H_

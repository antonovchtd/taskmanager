//
// Created by Anton O. on 12/6/21.
//

#ifndef TASKMANAGER_SRC_IO_ABSTRACTREADER_H_
#define TASKMANAGER_SRC_IO_ABSTRACTREADER_H_

#include <string>

class AbstractReader {
public:
    virtual std::string read(const std::string &prompt) = 0;
    virtual ~AbstractReader() = default;

};


#endif //TASKMANAGER_SRC_IO_ABSTRACTREADER_H_

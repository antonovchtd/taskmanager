//
// Created by Anton O. on 12/6/21.
//

#ifndef TASKMANAGER_SRC_IO_FILEREADER_H_
#define TASKMANAGER_SRC_IO_FILEREADER_H_

#include <fstream>
#include <sstream>

#include "AbstractReader.h"

class FileReader : public AbstractReader {
public:
    explicit FileReader(const std::string &filename);
    std::string read(const std::string &prompt) override;

private:
    std::string filename_;

};


#endif //TASKMANAGER_SRC_IO_FILEREADER_H_

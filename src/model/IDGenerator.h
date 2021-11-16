//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_IDGENERATOR_H_
#define TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

#include "TaskID.h"

class IDGenerator{
public:
    IDGenerator() : last_(0) {};
    explicit IDGenerator(unsigned int last) : last_(last) {};
    TaskID genID();
private:
    unsigned int last_;
};

#endif //TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

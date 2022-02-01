//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_IDGENERATOR_H_
#define TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

#include "Core.pb.h"

class IDGenerator{
public:
    IDGenerator();
    explicit IDGenerator(int last);
    IDGenerator(const IDGenerator&) = default;

    int state() const;

public:
    virtual Core::TaskID genID();
    virtual ~IDGenerator() = default;

protected:
    int last_;
};

#endif //TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

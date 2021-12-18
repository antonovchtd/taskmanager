//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_IDGENERATOR_H_
#define TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

#include "Task.pb.h"

class IDGenerator{
public:
    IDGenerator();
    explicit IDGenerator(int last);
    IDGenerator(const IDGenerator&) = default;

    int state() const;
    void setState(int last);

public:
    virtual ProtoTask::TaskID genID();
    virtual ~IDGenerator() = default;

protected:
    int last_;
};

#endif //TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

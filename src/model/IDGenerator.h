//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_IDGENERATOR_H_
#define TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

#include "Task.pb.h"

class IDGenerator{
public:
    IDGenerator();
    explicit IDGenerator(const ProtoTask::IDGeneratorState &last);
    IDGenerator(const IDGenerator&) = default;
    virtual ~IDGenerator() = default;

    ProtoTask::IDGeneratorState state() const;

    virtual ProtoTask::TaskID genID();

protected:
    ProtoTask::IDGeneratorState last_;
};

#endif //TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

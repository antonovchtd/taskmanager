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

    ProtoTask::IDGeneratorState state() const;

    ProtoTask::TaskID genID();

private:
    ProtoTask::IDGeneratorState last_;
};

#endif //TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

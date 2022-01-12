//
// Created by Anton O. on 1/5/22.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTERINTERFACE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTERINTERFACE_H_

#include <memory>
#include "Task.pb.h"
class ModelInterface;

class Persister {
public:
    virtual bool save(const std::vector<ProtoTask::TaskEntity> &) = 0;
    virtual bool load(std::vector<ProtoTask::TaskEntity> &) = 0;

public:
    virtual ~Persister() = default;

};

#endif //TASKMANAGER_SRC_PERSISTENCE_PERSISTERINTERFACE_H_



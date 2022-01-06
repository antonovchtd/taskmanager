//
// Created by Anton O. on 1/5/22.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTERINTERFACE_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTERINTERFACE_H_

#include <memory>
class TaskManagerInterface;

class PersisterInterface {
public:
    virtual bool save(const std::string &filename, const std::shared_ptr<TaskManagerInterface> &model) = 0;
    virtual bool load(const std::string &filename, const std::shared_ptr<TaskManagerInterface> &model) = 0;

public:
    virtual std::string defaultLocation() const = 0;

public:
    virtual ~PersisterInterface() = default;

};

#endif //TASKMANAGER_SRC_PERSISTENCE_PERSISTERINTERFACE_H_



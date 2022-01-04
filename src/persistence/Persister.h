//
// Created by Anton Ovcharenko on 15.12.2021.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_SAVER_H_
#define TASKMANAGER_SRC_PERSISTENCE_SAVER_H_

#include <memory>
#include <vector>
#include "Task.pb.h"
class TaskManager;

class Persister {
public:
    virtual bool save(const std::string &filename, const std::shared_ptr<TaskManager> &model);
    virtual bool load(const std::string &filename, const std::shared_ptr<TaskManager> &model);

public:
    ~Persister() = default;

public:
    std::string defaultFilename() const;

private:
    std::string default_filename_ = "data.bin";
};


#endif //TASKMANAGER_SRC_PERSISTENCE_SAVER_H_

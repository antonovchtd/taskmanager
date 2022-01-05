//
// Created by Anton Ovcharenko on 15.12.2021.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTER_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTER_H_

#include <vector>
#include "PersisterInterface.h"
#include "Task.pb.h"

class Persister : public PersisterInterface {
public:
    bool save(const std::string &filename, const std::shared_ptr<TaskManager> &model) override;
    bool load(const std::string &filename, const std::shared_ptr<TaskManager> &model) override;

public:
    std::string defaultFilename() const override;

private:
    std::string default_filename_ = "data.bin";
};


#endif //TASKMANAGER_SRC_PERSISTENCE_PERSISTER_H_

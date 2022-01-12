//
// Created by Anton Ovcharenko on 15.12.2021.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_PERSISTER_H_
#define TASKMANAGER_SRC_PERSISTENCE_PERSISTER_H_

#include <vector>
#include "Persister.h"
#include "Task.pb.h"

class FilePersistence : public Persister {
public:
    explicit FilePersistence(const std::string &filename);

public:
    bool save(const std::vector<Core::TaskEntity> &) override;
    bool load(std::vector<Core::TaskEntity> &) override;

private:
    std::string filename_;
};


#endif //TASKMANAGER_SRC_PERSISTENCE_PERSISTER_H_

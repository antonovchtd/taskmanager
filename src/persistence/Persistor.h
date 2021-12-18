//
// Created by Anton Ovcharenko on 15.12.2021.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_SAVER_H_
#define TASKMANAGER_SRC_PERSISTENCE_SAVER_H_

#include <memory>
#include <vector>
#include "../model/Task.pb.h"
class TaskManager;

class Persistor {
public:
    static bool save(const std::string &filename, const std::shared_ptr<TaskManager> &model);
    static bool load(const std::string &filename, const std::shared_ptr<TaskManager> &model);
};


#endif //TASKMANAGER_SRC_PERSISTENCE_SAVER_H_

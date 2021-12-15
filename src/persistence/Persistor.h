//
// Created by Anton Ovcharenko on 15.12.2021.
//

#ifndef TASKMANAGER_SRC_PERSISTENCE_SAVER_H_
#define TASKMANAGER_SRC_PERSISTENCE_SAVER_H_

#include <memory>
class TaskManager;

class Persistor {
public:
    static void save(const TaskManager &, std::ostream &);
    static TaskManager load(std::istream &);
};


#endif //TASKMANAGER_SRC_PERSISTENCE_SAVER_H_

//
// Created by Anton Ovcharenko on 17.01.2022.
//

#ifndef TASKMANAGER_SRC_UTILITIES_TASKENTITYUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_TASKENTITYUTILS_H_

#include "Core.pb.h"
#include "TaskUtils.h"

namespace Core {
bool operator==(const Core::TaskEntity &lhs, const Core::TaskEntity &rhs);
Core::TaskEntity createTaskEntity(const Core::TaskID &id,
                                  const Core::Task &task,
                                  const Core::TaskID &parent_it);
Core::TaskEntity createTaskEntity(const Core::TaskID &id,
                                  const Core::Task &task);
}


#endif //TASKMANAGER_SRC_UTILITIES_TASKENTITYUTILS_H_

//
// Created by Anton Ovcharenko on 17.01.2022.
//

#ifndef TASKMANAGER_SRC_UTILITIES_TASKENTITYUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_TASKENTITYUTILS_H_

#include "Task.pb.h"

namespace Core {
bool operator==(const Core::TaskEntity &lhs, const Core::TaskEntity &rhs);
}


#endif //TASKMANAGER_SRC_UTILITIES_TASKENTITYUTILS_H_

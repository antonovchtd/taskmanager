//
// Created by Anton Ovcharenko on 23.12.2021.
//

#ifndef TASKMANAGER_SRC_UTILITIES_TASKIDUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_TASKIDUTILS_H_

#include "Core.pb.h"

namespace Core {
    bool operator==(const Core::TaskID &lhs, const Core::TaskID &rhs);
    bool operator!=(const Core::TaskID &lhs, const Core::TaskID &rhs);
    bool operator<(const Core::TaskID &lhs, const Core::TaskID &rhs);
    bool operator>(const Core::TaskID &lhs, const Core::TaskID &rhs);
}
bool operator==(const std::vector<Core::TaskID> &lhs, const std::vector<Core::TaskID> &rhs);
bool operator!=(const std::vector<Core::TaskID> &lhs, const std::vector<Core::TaskID> &rhs);

#endif //TASKMANAGER_SRC_UTILITIES_TASKIDUTILS_H_

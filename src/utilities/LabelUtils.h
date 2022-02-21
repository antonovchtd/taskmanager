//
// Created by Anton O. on 2/21/22.
//

#ifndef TASKMANAGER_SRC_UTILITIES_LABELUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_LABELUTILS_H_

#include "Core.pb.h"

namespace Core {
    bool operator==(const Core::Label &lhs, const Core::Label &rhs);
    bool operator!=(const Core::Label &lhs, const Core::Label &rhs);
    bool operator==(const ::google::protobuf::RepeatedPtrField<Core::Label> &lhs,
                    const ::google::protobuf::RepeatedPtrField<Core::Label> &rhs);
};


#endif //TASKMANAGER_SRC_UTILITIES_LABELUTILS_H_

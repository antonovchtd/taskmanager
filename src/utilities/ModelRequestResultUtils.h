//
// Created by Anton Ovcharenko on 01.02.2022.
//

#ifndef TASKMANAGER_SRC_UTILITIES_MODELINQUIRYRESULTUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_MODELINQUIRYRESULTUTILS_H_

#include "Core.pb.h"

namespace Core {
    bool ToBool(const Core::ModelRequestResult &result);
    std::string ToString(const Core::ModelRequestResult::Status &status);
    std::string ToString(const Core::ModelRequestResult::Status &status,
                         const Core::TaskID &id);
};


#endif //TASKMANAGER_SRC_UTILITIES_MODELINQUIRYRESULTUTILS_H_

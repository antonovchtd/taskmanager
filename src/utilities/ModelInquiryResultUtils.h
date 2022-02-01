//
// Created by Anton Ovcharenko on 01.02.2022.
//

#ifndef TASKMANAGER_SRC_UTILITIES_MODELINQUIRYRESULTUTILS_H_
#define TASKMANAGER_SRC_UTILITIES_MODELINQUIRYRESULTUTILS_H_

#include "Core.pb.h"

namespace Core {
    bool ToBool(const Core::ModelInquiryResult &result);
    std::string ToString(const Core::ModelInquiryResult::Status &status);
    std::string ToString(const Core::ModelInquiryResult::Status &status,
                         const Core::TaskID &id);
};


#endif //TASKMANAGER_SRC_UTILITIES_MODELINQUIRYRESULTUTILS_H_

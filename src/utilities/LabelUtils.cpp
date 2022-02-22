//
// Created by Anton O. on 2/21/22.
//

#include "LabelUtils.h"

bool Core::operator==(const Core::Label &lhs, const Core::Label &rhs) {
    return lhs.str() == rhs.str();
}

bool Core::operator!=(const Core::Label &lhs, const Core::Label &rhs) {
    return lhs.str() != rhs.str();
}

bool Core::operator==(const ::google::protobuf::RepeatedPtrField<Core::Label> &lhs,
                const ::google::protobuf::RepeatedPtrField<Core::Label> &rhs) {
    if (lhs.size() == rhs.size()) {
        for (int i = 0; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }
    } else {
        return false;
    }

    return true;
}
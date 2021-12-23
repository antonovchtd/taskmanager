//
// Created by Anton Ovcharenko on 23.12.2021.
//

#include "NodeUtils.h"
#include "../model/Node.h"
#include "TaskIDUtils.h"

bool operator==(const Node &lhs, const Node &rhs) {
    if (rhs.children() != lhs.children())
        return false;
    else {
        if (lhs.parent().has_value() && rhs.parent().has_value()) {
            return *lhs.parent() == *rhs.parent();
        } else if (!(!lhs.parent().has_value() && !rhs.parent().has_value())) {
            return false;
        }
    }
    return true;
}
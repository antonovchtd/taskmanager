//
// Created by Anton Ovcharenko on 17.01.2022.
//

#include "TaskEntityUtils.h"

bool Core::operator==(const Core::TaskEntity &lhs, const Core::TaskEntity &rhs) {
    return lhs.id().value() == rhs.id().value() &&
           lhs.data().title() == rhs.data().title() &&
           lhs.data().due_date() == rhs.data().due_date() &&
           lhs.data().priority() == rhs.data().priority() &&
           lhs.data().is_complete() == rhs.data().is_complete() &&
           lhs.data().labels() == rhs.data().labels() &&
           lhs.parent().value() == rhs.parent().value();
}
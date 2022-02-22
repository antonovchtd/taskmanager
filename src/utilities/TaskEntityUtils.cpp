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

Core::TaskEntity  Core::createTaskEntity(const Core::TaskID &id,
                                        const Core::Task &task,
                                        const Core::TaskID &parent_id) {
    Core::TaskEntity te;
    te.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    te.set_allocated_data(std::make_unique<Core::Task>(task).release());
    te.set_allocated_parent(std::make_unique<Core::TaskID>(parent_id).release());
    return te;
}
Core::TaskEntity  Core::createTaskEntity(const Core::TaskID &id,
                                         const Core::Task &task) {
    Core::TaskEntity te;
    te.set_allocated_id(std::make_unique<Core::TaskID>(id).release());
    te.set_allocated_data(std::make_unique<Core::Task>(task).release());
    return te;
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "EditTaskAction.h"

EditTaskAction::EditTaskAction(const Core::TaskID &id, const Core::Task &task) :
                id_{id}, task_{task} {
}

ActionResult EditTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->Edit(id_, task_);
}
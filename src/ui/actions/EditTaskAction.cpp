//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "EditTaskAction.h"

EditTaskAction::EditTaskAction(const Core::Task &task, const Core::TaskID &id) :
        task_{task}, id_{id} {
}

ActionResult EditTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->Edit(id_, task_);
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddTaskAction.h"

AddTaskAction::AddTaskAction(const Core::Task &task) : task_{task} {
}

ActionResult AddTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->Add(task_);
}
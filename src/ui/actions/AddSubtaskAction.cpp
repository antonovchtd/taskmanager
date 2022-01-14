//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddSubtaskAction.h"

AddSubtaskAction::AddSubtaskAction(const Core::Task &task, const Core::TaskID &id) :
                  task_{task}, id_{id} {
}

ActionResult AddSubtaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->AddSubtask(task_, id_);
}
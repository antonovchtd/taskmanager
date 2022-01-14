//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "AddSubtaskAction.h"

AddSubtaskAction::AddSubtaskAction(const Core::TaskID &id, const Core::Task &task) :
                  id_{id}, task_{task} {
}

ActionResult AddSubtaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->AddSubtask(task_, id_);
}
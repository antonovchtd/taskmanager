//
// Created by Anton O. on 1/17/22.
//

#include "UnlabelAllTasksAction.h"
#include "ui/Context.h"

UnlabelAllTasksAction::UnlabelAllTasksAction(const Core::TaskID &id) : id_{id}{
}

ActionResult UnlabelAllTasksAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->ClearLabels(id_);
}
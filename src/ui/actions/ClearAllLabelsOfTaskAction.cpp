//
// Created by Anton O. on 1/17/22.
//

#include "ClearAllLabelsOfTaskAction.h"
#include "ui/Context.h"

ClearAllLabelsOfTaskAction::ClearAllLabelsOfTaskAction(const Core::TaskID &id) : id_{id}{
}

ActionResult ClearAllLabelsOfTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->ClearLabels(id_);
}
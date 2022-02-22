//
// Created by Anton O. on 1/17/22.
//

#include "ClearLabelOfTaskAction.h"
#include "ui/Context.h"

ClearLabelOfTaskAction::ClearLabelOfTaskAction(const Core::TaskID &id, const Core::Label &label) :
        id_{id}, label_{label} {
}

ActionResult ClearLabelOfTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->RemoveLabel(id_, label_);
}

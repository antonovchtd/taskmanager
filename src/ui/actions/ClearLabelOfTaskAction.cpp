//
// Created by Anton O. on 1/17/22.
//

#include "ClearLabelOfTaskAction.h"
#include "ui/Context.h"

ClearLabelOfTaskAction::ClearLabelOfTaskAction(const Core::TaskID &id, const std::string &label) :
        id_{id}, label_{label} {
}

ActionResult ClearLabelOfTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->ClearLabel(id_, label_);
}

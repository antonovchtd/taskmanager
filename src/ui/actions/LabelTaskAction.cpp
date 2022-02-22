//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "LabelTaskAction.h"

LabelTaskAction::LabelTaskAction(const Core::TaskID &id, const Core::Label &label) :
                 id_{id}, label_{label} {
}

ActionResult LabelTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->AddLabel(id_, label_);
}
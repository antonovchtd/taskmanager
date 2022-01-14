//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "LabelTaskAction.h"

LabelTaskAction::LabelTaskAction(const std::string &label) : label_{label} {
}

ActionResult LabelTaskAction::execute(Context &context, const std::shared_ptr<ModelInterface> &model) {
    return model->AddLabel(*context.id(), label_);
}
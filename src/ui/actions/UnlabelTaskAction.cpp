//
// Created by Anton O. on 1/17/22.
//

#include "UnlabelTaskAction.h"
#include "ui/Context.h"

UnlabelTaskAction::UnlabelTaskAction(const Core::TaskID &id, const std::string &label) :
        id_{id}, label_{label} {
}

ActionResult UnlabelTaskAction::execute(const std::shared_ptr<ModelInterface> &model) {
    return model->ClearLabel(id_, label_);
}
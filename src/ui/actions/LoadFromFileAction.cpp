//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "LoadFromFileAction.h"
#include "persistence/FilePersistence.h"
#include "ui/Context.h"

LoadFromFileAction::LoadFromFileAction(const std::string &filename) : filename_{filename} {
}

ActionResult LoadFromFileAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelRequestResult result;

    if (filename_.empty()) {
        result.set_status(Core::ModelRequestResult_Status_TAKES_ARG);
        return result;
    }

    persister_ = std::make_unique<FilePersistence>(filename_);
    std::vector<Core::TaskEntity> data;
    if (persister_->load(data)) {
        model->Replace(data);
        result.set_status(Core::ModelRequestResult_Status_SUCCESS);
    }
    else {
        result.set_status(Core::ModelRequestResult_Status_FILE_NOT_FOUND);
    }

    return result;
}
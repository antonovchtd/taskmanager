//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "LoadFromFileAction.h"
#include "persistence/FilePersistence.h"
#include "ui/Context.h"

LoadFromFileAction::LoadFromFileAction(const std::string &filename) : filename_{filename} {
}

ActionResult LoadFromFileAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (filename_.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    persister_ = std::unique_ptr<Persister>(new FilePersistence{filename_});
    std::vector<Core::TaskEntity> data;
    if (persister_->load(data)) {
        model->Replace(data);
        return {ActionResult::Status::SUCCESS, std::nullopt};
    }
    else
        return {ActionResult::Status::FILE_NOT_FOUND, std::nullopt};
}
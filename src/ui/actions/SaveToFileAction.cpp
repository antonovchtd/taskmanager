//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "SaveToFileAction.h"
#include "persistence/FilePersistence.h"

SaveToFileAction::SaveToFileAction(const std::string &filename) : filename_{filename} {
}

ActionResult SaveToFileAction::execute(const std::shared_ptr<ModelInterface> &model) {
    if (filename_.empty())
        return {ActionResult::Status::TAKES_ARG, std::nullopt};
    persister_ = std::unique_ptr<Persister>(new FilePersistence{filename_});
    if (persister_->save(model->getTasks()))
        return {ActionResult::Status::SUCCESS, std::nullopt};
    else
        return {ActionResult::Status::FAILED_TO_OPEN_FILE, std::nullopt};
}
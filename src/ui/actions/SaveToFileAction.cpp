//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "SaveToFileAction.h"
#include "persistence/FilePersistence.h"

SaveToFileAction::SaveToFileAction(const std::string &filename) : filename_{filename} {
}

ActionResult SaveToFileAction::execute(const std::shared_ptr<ModelInterface> &model) {
    Core::ModelInquiryResult result;

    if (filename_.empty()) {
        result.set_status(Core::ModelInquiryResult_Status_TAKES_ARG);
        return result;
    }

    persister_ = std::unique_ptr<Persister>(new FilePersistence{filename_});
    if (persister_->save(model->getTasks()))
        result.set_status(Core::ModelInquiryResult_Status_SUCCESS);
    else
        result.set_status(Core::ModelInquiryResult_Status_FAILED_TO_OPEN_FILE);

    return result;
}
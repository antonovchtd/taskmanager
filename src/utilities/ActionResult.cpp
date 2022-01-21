//
// Created by Anton O. on 12/18/21.
//

#include "ActionResult.h"

ActionResult::ActionResult(Status s, const std::optional<Core::TaskID> &tid) :
              status{s}, id{tid}, type_id{kID} {
}

ActionResult::ActionResult(Status s, const Core::TaskEntity &te) :
              status{s}, entity{te}, type_id{kEntity} {
}

ActionResult::ActionResult(Status s, const std::vector<Core::TaskEntity> &vec) :
              status{s}, tasks{vec}, type_id{kVector} {
}

ActionResult::~ActionResult() {
    switch (type_id) {
        case kID:
            if (id)
                id->Core::TaskID::~TaskID();
            break;
        case kEntity:
            entity.Core::TaskEntity::~TaskEntity();
            break;
        case kVector:
            break;
    }
}

ActionResult::operator bool() const {
    return status == ActionResult::Status::SUCCESS;
}

std::string ActionResult::message() const {
    switch (status) {
        case Status::SUCCESS:
            return "Success!";
        case Status::ID_NOT_FOUND:
            return "ID " + std::to_string(id->value()) + " was not found.\n";
        case Status::PARENT_ID_NOT_FOUND:
            return "Parent ID " + std::to_string(id->value()) + " was not found.\n";
        case Status::HAS_CHILDREN:
            return "Cannot proceed: Task " + std::to_string(id->value()) + " has children.\n";
        case Status::DUPLICATE_ID:
            return "Error: Duplicate ID " + std::to_string(id->value()) + ".\n";
        case Status::FILE_NOT_FOUND:
            return "File could not be found.\n";
        case Status::FAILED_TO_OPEN_FILE:
            return "Failed to open file.\n";
        case Status::TAKES_NO_ARG:
            return "This function takes no argument.\n";
        case Status::TAKES_ARG:
            return "This function requires an argument.\n";
        case Status::TAKES_ID:
            return "This function requires an ID argument.\n";
        case Status::TAKES_ALPHA_NOT_ID:
            return "This function requires a word, not an ID argument.\n";
        case Status::TAKES_ALPHA:
            return "This function requires an alpha argument.\n";
    }

}
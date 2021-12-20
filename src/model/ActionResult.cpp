//
// Created by Anton O. on 12/18/21.
//

#include "ActionResult.h"

ActionResult::operator bool() const {
    return status == ActionResult::Status::SUCCESS;
}

std::string ActionResult::message() const {
    switch (status) {
        case Status::SUCCESS:
            return "Success!";
        case Status::ID_NOT_FOUND:
            return "ID " + std::to_string(id->num()) + " was not found.\n";
        case Status::PARENT_ID_NOT_FOUND:
            return "Parent ID " + std::to_string(id->num()) + " was not found.\n";
        case Status::HAS_CHILDREN:
            return "Cannot proceed: Task " + std::to_string(id->num()) + " has children.\n";
        case Status::DUPLICATE_ID:
            return "Error: Duplicate ID " + std::to_string(id->num()) + ".\n";
        case Status::FILE_NOT_FOUND:
            return "File could not be found.\n";
        case Status::FAILED_TO_OPEN_FILE:
            return "Failed to open file.\n";
        case Status::TAKES_NO_ARG:
            return "This function takes no argument.\n";
        case Status::TAKES_ARG:
            return "This function requires an argument.\n";
        case Status::NOT_AN_ID:
            return "This function requires an ID argument.\n";
    }

}
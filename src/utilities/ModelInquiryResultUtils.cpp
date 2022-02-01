//
// Created by Anton Ovcharenko on 01.02.2022.
//

#include "ModelInquiryResultUtils.h"

bool Core::ToBool(const Core::ModelInquiryResult &result) {
    return result.has_id() || result.status() == Core::ModelInquiryResult_Status_SUCCESS;
}

std::string Core::ToString(const Core::ModelInquiryResult::Status &status) {
    switch (status) {
        case Core::ModelInquiryResult_Status_SUCCESS:
            return "Success!";
        case Core::ModelInquiryResult_Status_ID_NOT_FOUND:
            return "ID was not found.\n";
        case Core::ModelInquiryResult_Status_PARENT_ID_NOT_FOUND:
            return "Parent ID was not found.\n";
        case Core::ModelInquiryResult_Status_HAS_CHILDREN:
            return "Cannot proceed: Task has children.\n";
        case Core::ModelInquiryResult_Status_DUPLICATE_ID:
            return "Error: Duplicate ID.\n";
        case Core::ModelInquiryResult_Status_FILE_NOT_FOUND:
            return "File could not be found.\n";
        case Core::ModelInquiryResult_Status_FAILED_TO_OPEN_FILE:
            return "Failed to open file.\n";
        case Core::ModelInquiryResult_Status_TAKES_NO_ARG:
            return "This function takes no argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ARG:
            return "This function requires an argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ID:
            return "This function requires an ID argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ALPHA_NOT_ID:
            return "This function requires a word, not an ID argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ALPHA:
            return "This function requires an alpha argument.\n";
        default:
            return "";
    }
}

std::string Core::ToString(const Core::ModelInquiryResult::Status &status,
                           const Core::TaskID &id) {
    switch (status) {
        case Core::ModelInquiryResult_Status_SUCCESS:
            return "Success!";
        case Core::ModelInquiryResult_Status_ID_NOT_FOUND:
            return "ID " + std::to_string(id.value()) + " was not found.\n";
        case Core::ModelInquiryResult_Status_PARENT_ID_NOT_FOUND:
            return "Parent ID " + std::to_string(id.value()) + " was not found.\n";
        case Core::ModelInquiryResult_Status_HAS_CHILDREN:
            return "Cannot proceed: Task " + std::to_string(id.value()) + " has children.\n";
        case Core::ModelInquiryResult_Status_DUPLICATE_ID:
            return "Error: Duplicate ID " + std::to_string(id.value()) + ".\n";
        case Core::ModelInquiryResult_Status_FILE_NOT_FOUND:
            return "File could not be found.\n";
        case Core::ModelInquiryResult_Status_FAILED_TO_OPEN_FILE:
            return "Failed to open file.\n";
        case Core::ModelInquiryResult_Status_TAKES_NO_ARG:
            return "This function takes no argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ARG:
            return "This function requires an argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ID:
            return "This function requires an ID argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ALPHA_NOT_ID:
            return "This function requires a word, not an ID argument.\n";
        case Core::ModelInquiryResult_Status_TAKES_ALPHA:
            return "This function requires an alpha argument.\n";
        default:
            return "";
    }
}
//
// Created by Anton O. on 12/18/21.
//

#include "ActionResult.h"
#include "utilities/ModelInquiryResultUtils.h"

ActionResult::ActionResult(const Core::ModelInquiryResult &result) :
              model_result{result}, type_id{kResult} {
}

ActionResult::ActionResult(const Core::TaskEntity &te) :
              entity{te}, type_id{kEntity} {
}

ActionResult::ActionResult(const std::vector<Core::TaskEntity> &vec) :
              tasks{vec}, type_id{kVector} {
}

ActionResult::~ActionResult() {
    switch (type_id) {
        case kResult:
            (&model_result)->Core::ModelInquiryResult::~ModelInquiryResult();
            break;
        case kEntity:
            (&entity)->Core::TaskEntity::~TaskEntity();
            break;
        case kVector:
            (&tasks)->~vector();
            break;
    }
}

ActionResult::operator bool() const {
    switch (type_id) {
        case kResult:
            return ToBool(model_result);
        default:
            return true;
    }
}
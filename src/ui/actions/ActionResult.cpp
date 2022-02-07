//
// Created by Anton O. on 12/18/21.
//

#include "ActionResult.h"
#include "utilities/ModelRequestResultUtils.h"

ActionResult::ActionResult(const Core::ModelRequestResult &result) : model_result{result} {
}

ActionResult::ActionResult(const Core::TaskEntity &te) : entity{te} {
}

ActionResult::ActionResult(const std::vector<Core::TaskEntity> &vec) : tasks{vec} {
}

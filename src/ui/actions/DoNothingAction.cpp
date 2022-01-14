//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "DoNothingAction.h"

ActionResult DoNothingAction::execute(Context &, const std::shared_ptr<ModelInterface> &) {
    return {ActionResult::Status::SUCCESS, std::nullopt};
}
//
// Created by Anton O. on 11/30/21.
//

#include "Controller.h"

void Controller::Accept(TaskManager &model, Context &c, const std::shared_ptr<Action> &act) {
    act->make(model, c);
}
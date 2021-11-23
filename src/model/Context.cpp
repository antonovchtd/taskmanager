//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"

void Context::execute(){
    state_->execute(*this);
}

void Context::changeState(std::shared_ptr<State> s) {
    state_ = s;
}
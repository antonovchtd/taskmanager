//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Context Machine::run(std::optional<StepFactory::State> state) {
    if (state)
        context_.changeStep(factory_.getStep(state.value()));
    else
        context_.changeStep(factory_.nextStep());
    while (context_.getStep()){
        context_.execute(factory_);
    }
    return context_;
}
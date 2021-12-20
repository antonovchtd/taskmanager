//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Machine::Machine() : initial_step_{Factory::State::HOME} {
    factory_ = Factory::create();
}

Machine::Machine(const std::shared_ptr<Factory> &f, const Factory::State &s) : factory_{f}, initial_step_{s} {
}

Context Machine::run() {

    auto step = factory_->lazyInitStep(initial_step_);
    while (step){
        step = step->execute(context_);
//        context_.step()->process(context_);
    }
    return context_;
}

std::shared_ptr<TaskManager> Machine::model() const {
    return factory_->model();
}
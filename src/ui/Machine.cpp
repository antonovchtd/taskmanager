//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Machine::Machine() {
    factory_ = Factory::create();
    context_.setStep(factory_->lazyInitStep(Factory::State::HOME));
}

Machine::Machine(const Factory::State &s) : Machine() {
    context_.setStep(factory_->lazyInitStep(s));
}

Machine::Machine(const std::shared_ptr<Factory> &f, const Factory::State &s) : factory_(f) {
    context_.setStep(factory_->lazyInitStep(s));
}

Context Machine::run() {

    while (context_.step()){
        auto act = context_.step()->execute(context_);
        act->make(context_);
        context_.step()->process(context_);
    }
    return context_;
}

std::shared_ptr<TaskManager> Machine::model() const {
    return factory_->model();
}
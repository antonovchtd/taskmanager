//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Machine::Machine() {
    factory_ = Factory::create();
    context_.setStep(factory_->getStep(Factory::State::HOME));
}

Machine::Machine(const Factory::State &s) {
    factory_ = Factory::create();
    context_.setStep(factory_->getStep(s));
}

Machine::Machine(const std::shared_ptr<Factory> &f, const Factory::State &s) : factory_(f) {
    context_.setStep(factory_->getStep(s));
}

Context Machine::run() {

    while (context_.getStep()){
        auto act = context_.getStep()->execute(context_);
        if (act) {
            act->make(model_, context_);
        }
        context_.getStep()->process(context_);
    }
    return context_;
}

TaskManager Machine::model() const {
    return model_;
}
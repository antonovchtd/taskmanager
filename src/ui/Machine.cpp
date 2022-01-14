//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Machine::Machine() : initial_step_{Factory::State::HOME},
                     model_{std::shared_ptr<ModelInterface>(new TaskManager)} {
    factory_ = Factory::create();
}

Machine::Machine(const std::shared_ptr<Factory> &f, const Factory::State &s) :
        factory_{f}, initial_step_{s},
        model_{std::shared_ptr<ModelInterface>(new TaskManager)} {
}

Context Machine::run() {
    return run(context_);
}

Context Machine::run(Context &context) {

    auto step = factory_->lazyInitStep(initial_step_);
    while (step){
        auto action = step->genAction(context);
        auto result = action->execute(context, model_);
        step = step->genNextStep(result, factory_);
    }
    return context;
}

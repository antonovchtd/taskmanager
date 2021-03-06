//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Machine::Machine(const std::shared_ptr<ModelInterface> &model) :
                 initial_step_{Factory::State::HOME},
                 model_{model},
                 factory_{Factory::create()} {
}

Machine::Machine(const std::shared_ptr<ModelInterface> &model, const std::shared_ptr<Factory> &f, const Factory::State &s) :
        factory_{f}, initial_step_{s}, model_{model} {
}

Machine::Machine(const std::shared_ptr<Factory> &f, const Factory::State &s) :
        factory_{f}, initial_step_{s} {
}

Context Machine::run() {
    return run(context_);
}

void updateContext(Context &context, const ActionResult &result) {
    if (result.model_result && result.model_result->has_id())
        context.setID(result.model_result->id());

    if (result.entity)
        context.setTasks(std::vector<Core::TaskEntity>{*result.entity});

    if (result.tasks) {
        context.setTasks(*result.tasks);
        if (!result.tasks->empty()) {
            context.setTask((*result.tasks)[0].data());
            context.setID((*result.tasks)[0].id());
        }
    }
}

Context Machine::run(const Context &context) {

    Context local_context = context;
    auto step = factory_->lazyInitStep(initial_step_);
    while (step){
        auto action = step->genAction(local_context);
        auto result = action->execute(model_);
        updateContext(local_context, result); // pass data to next step
        step = step->genNextStep(result, factory_);
    }
    return local_context;
}

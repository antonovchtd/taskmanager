//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

Context Machine::run(std::optional<StepFactory::State> state) {
    Context c;
    StepFactory f;
    if (state)
        c.changeStep(f.getStep(state.value()));
    else
        c.changeStep(f.nextStep());
    while (c.getStep()){
        c.execute(f);
    }
    return c;
}
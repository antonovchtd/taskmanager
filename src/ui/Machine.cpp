//
// Created by Anton O. on 11/26/21.
//

#include "Machine.h"

void Machine::run() {
    Context c;
    StepFactory f;
    c.changeStep(f.nextStep());
    while (c.getStep()){
        c.execute(f);
    }
}
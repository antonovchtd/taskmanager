//
// Created by Anton O. on 11/16/21.
//

#include "IDGenerator.h"

IDGenerator::IDGenerator() : last_(1) {
}

IDGenerator::IDGenerator(int last) : last_(last) {
}

TaskID IDGenerator::genID() {
    return TaskID(last_++);
}
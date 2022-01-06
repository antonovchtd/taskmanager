//
// Created by Anton O. on 11/16/21.
//

#include "IDGenerator.h"

IDGenerator::IDGenerator() : last_(1) {
}

IDGenerator::IDGenerator(int last) : last_(last) {
}

ProtoTask::TaskID IDGenerator::genID() {
    ProtoTask::TaskID id;
    id.set_value(last_++);
    return id;
}

int IDGenerator::state() const {
    return last_;
}

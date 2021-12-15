//
// Created by Anton O. on 11/16/21.
//

#include "IDGenerator.h"

IDGenerator::IDGenerator() {
    last_.set_value(1);
}

IDGenerator::IDGenerator(const ProtoTask::IDGeneratorState &last) : last_(last) {
}

ProtoTask::TaskID IDGenerator::genID() {
    ProtoTask::TaskID id;
    id.set_num(last_.value());
    last_.set_value(last_.value()+1);
    return id;
}

ProtoTask::IDGeneratorState IDGenerator::state() const {
    return last_;
}
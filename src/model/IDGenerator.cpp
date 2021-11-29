//
// Created by Anton O. on 11/16/21.
//

#include "IDGenerator.h"

TaskID IDGenerator::genID() {
    return TaskID(last_++);
}
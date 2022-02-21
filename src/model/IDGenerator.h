//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_IDGENERATOR_H_
#define TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

#include <atomic>
#include "Task.pb.h"

class IDGenerator{
public:
    IDGenerator();
    explicit IDGenerator(int last);

    int state() const;

public:
    virtual Core::TaskID genID();
    virtual ~IDGenerator() = default;

protected:
    std::atomic<int> last_;
};

#endif //TASKMANAGER_SRC_MODEL_IDGENERATOR_H_

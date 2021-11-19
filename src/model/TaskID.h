//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKID_H_
#define TASKMANAGER_SRC_MODEL_TASKID_H_

class TaskID{
public:
    explicit TaskID(unsigned int val) : value_(val) { };
    unsigned int value() const;
    bool operator==(const TaskID&) const;
    bool operator!=(const TaskID&) const;
    bool operator<(const TaskID&) const;

private:
    unsigned int value_;
};

#endif //TASKMANAGER_SRC_MODEL_TASKID_H_

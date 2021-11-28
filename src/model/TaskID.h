//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKID_H_
#define TASKMANAGER_SRC_MODEL_TASKID_H_

#include <iostream>
#include <optional>

class TaskID{
public:
    explicit TaskID(unsigned int val) : value_(val) { };
    static std::optional<TaskID> invalidID();

    unsigned int value() const;
    std::string str() const;

    friend std::istream& operator>>(std::istream& is, TaskID& id);
    bool operator==(const TaskID&) const;
    bool operator!=(const TaskID&) const;
    bool operator<(const TaskID&) const;

private:
    unsigned int value_;
};

#endif //TASKMANAGER_SRC_MODEL_TASKID_H_

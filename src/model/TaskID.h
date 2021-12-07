//
// Created by Anton O. on 11/16/21.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKID_H_
#define TASKMANAGER_SRC_MODEL_TASKID_H_

#include <iostream>
#include <string>
#include <optional>

class TaskID{
public:
    explicit TaskID(int val);
    static TaskID Create(const std::string &);
    static TaskID invalidID();
    static TaskID nullid();

    bool isValid() const;
    bool isValidOrNull() const;
    int value() const;
    std::string to_string() const;

    bool operator==(const TaskID&) const;
    bool operator!=(const TaskID&) const;
    bool operator<(const TaskID&) const;

private:
    int value_;
};

#endif //TASKMANAGER_SRC_MODEL_TASKID_H_

//
// Created by Anton O. on 11/19/21.
//

#ifndef TASKMANAGER_SRC_MODEL_PROMPT_H_
#define TASKMANAGER_SRC_MODEL_PROMPT_H_

#include <iostream>
#include <string>

#include "TaskManager.h"

class Prompt{
public:
    enum class State{
        EMPTY,
        TASK,
        SUBTASK,
        EDIT,
        LABEL
    };
    bool quit_flag() const { return quit_flag_; }
    void ask(std::ostream &);
    void input(std::ostream&, std::istream&);
    void act(std::ostream &, std::istream&, TaskManager &);

private:
    State state_ = State::EMPTY;
    std::string instr_;
    bool quit_flag_ = false;
    std::string title_;
    Task::Priority priority_;
    std::string due_date_;
    TaskID curr_id_ = TaskID::invalidID();
};

#endif //TASKMANAGER_SRC_MODEL_PROMPT_H_

//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "State.h"
#include "StateFactory.h"

void State::changeState(const std::shared_ptr<Context> &c, const std::shared_ptr<State> &s) {
    c->changeState(s);
}

void State::printline(const std::string &line){
    std::cout << line;
}

std::string State::readline(const std::string &prompt) {
    State::printline(prompt);
    std::string input;
    getline(std::cin, input);
    return input;
}

void HomeState::execute(Context &c, StateFactory &f) {
    std::string command = State::readline(" > ");
    c.changeState(f.create(command));
}

void HelpState::execute(Context &c, StateFactory &f) {
    std::ifstream file("../src/model/help.txt");
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            State::printline(line + "\n");
        file.close();
    }
    c.changeState(f.create("HomeState"));
}

void QuitState::execute(Context &c, StateFactory &f) {
    c.changeState(nullptr);
}

void AddState::execute(Context &c, StateFactory &f) {
    State::printline("[Add Task]\n");
    c.changeState(f.create("ReadTitleState"));
}

void ReadTitleState::execute(Context &c, StateFactory &f) {
    std::string title;
    while (true){
        title = State::readline("    Title > ");
        if (title.empty())
            State::printline("Title cannot be empty!\n");
        else
            break;
    }
    c.setTitle(title);
    c.changeState(f.create("ReadPriorityState"));
}

void ReadPriorityState::execute(Context &c, StateFactory &f) {
    std::string p;
    int pint;
    while (true){
        p = State::readline("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ");
        pint = std::atoi(p.c_str());
        if (pint >= 0 && pint <= 3)
            break;
        else
            State::printline("    Wrong priority option. Try again.\n");
    }
    c.setPriority(static_cast<Task::Priority>(pint));
    c.changeState(f.create("ReadDueDateState"));
}

void ReadDueDateState::execute(Context &c, StateFactory &f) {
    std::optional<time_t> due_date;
    while (true) {
        std::string datestring = State::readline("    Due > ");
        due_date = Task::stringToTime(datestring);
        if (due_date)
            break;
        else
            State::printline("    Wrong date format. Try again.\n");
    }
    c.setDueDate(due_date.value());
    c.changeState(f.create("AddTaskState"));
}

void AddTaskState::execute(Context &c, StateFactory &f) {
    c.man_->Add(Task::Create(c.data_));
    c.changeState(f.create("HomeState"));
}

void ShowState::execute(Context &c, StateFactory &f) {
    c.man_->Show(std::cout);
    c.changeState(f.create("HomeState"));
}
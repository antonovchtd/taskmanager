//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "State.h"
#include "StateFactory.h"

void State::changeState(const std::shared_ptr<Context> &c, std::shared_ptr<State> s) {
    c->changeState(s);
}

std::string State::readline(const std::string &prompt) {
    std::cout << prompt;
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
    if (file.is_open()) {
        std::cout << file.rdbuf();
        file.close();
    }
    c.changeState(f.create("HomeState"));
}

void QuitState::execute(Context &c, StateFactory &f) {
    c.changeState(nullptr);
}

void AddState::execute(Context &c, StateFactory &f) {
    std::cout << "[Add Task]\n";
    c.changeState(f.create("ReadTitleState"));
}

void ReadTitleState::execute(Context &c, StateFactory &f) {
    std::string title;
    while (true){
        title = State::readline("    Title > ");
        if (title.empty())
            std::cout << "Title cannot be empty!\n";
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
            std::cout << "    Wrong priority option. Try again.\n";
    }
    c.setPriority(static_cast<Task::Priority>(pint));
    c.changeState(f.create("ReadDueDateState"));
}

void ReadDueDateState::execute(Context &c, StateFactory &f) {
    std::string dd = State::readline("    Due > ");
    c.setDueDate(dd);
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
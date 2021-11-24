//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "State.h"

void State::changeState(const std::shared_ptr<Context> &c, std::shared_ptr<State> s) {
    c->changeState(s);
}

std::string State::readline(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    std::cin >> input;
    return input;
}

void HomeState::execute(Context & c) {
    std::string command = State::readline(" > ");
    std::shared_ptr<State> nextState = StateFactory::create(command);
    c.changeState(nextState);
}

void HelpState::execute(Context &c){
    std::ifstream f("../src/model/help.txt");
    if (f.is_open()) {
        std::cout << f.rdbuf();
        f.close();
    }
    c.changeState(std::shared_ptr<State>{new HomeState});
}

void QuitState::execute(Context &c){
    c.changeState(nullptr);
}

void AddState::execute(Context &c) {
    std::cout << "[Add Task]\n";
    c.changeState(std::shared_ptr<State>{new ReadTitleState});
}

void ReadTitleState::execute(Context &c) {
    std::string title;
    while (true){
        title = State::readline("    Title > ");
        if (title.empty())
            std::cout << "Title cannot be empty!\n";
        else
            break;
    }
    c.setTitle(title);
    c.changeState(std::shared_ptr<State>{new ReadPriorityState});
}

void ReadPriorityState::execute(Context &c) {
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
    c.changeState(std::shared_ptr<State>{new ReadDueDateState});
}

void ReadDueDateState::execute(Context &c) {
    std::string dd = State::readline("    Due > ");
    c.setDueDate(dd);
    c.changeState(std::shared_ptr<State>{new AddTaskState});
}

void AddTaskState::execute(Context &c) {
    c.man_->Add(Task::Create(c.title_, c.priority_, c.due_date_, false));
    c.changeState(std::shared_ptr<State>{new HomeState});
}

void ShowState::execute(Context &c) {
    c.man_->Show(std::cout);
    c.changeState(std::shared_ptr<State>{new HomeState});
}
//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "State.h"

void State::changeState(std::shared_ptr<Context> c, std::shared_ptr<State> s) {
    c->changeState(s);
}

std::string State::readline(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    std::cin >> input;
    return input;
}

void HomeState::execute(Context & c) {
    std::string input = readline(" > ");
    std::shared_ptr<State> nextState;
    if (input == "add"){
//        nextState = std::shared_ptr<State>{new AddState};
    }
    else if (input == "help"){
        nextState = std::shared_ptr<State>{new HelpState};
    }
    else if (input == "quit"){
        nextState = std::shared_ptr<State>{new QuitState};
    }
    else{
        std::cout << "Wrong command. Try again. Type `help` for help.\n";
        nextState = std::shared_ptr<State>{new HomeState};
    }
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

}
//
// Created by Anton O. on 11/23/21.
//

#include "StateFactory.h"

std::shared_ptr<State> StateFactory::create(const std::string &command) {
    if (command == "add"){
        return std::shared_ptr<State>{new AddState};
    }
    else if (command == "help"){
        return std::shared_ptr<State>{new HelpState};
    }
    else if (command == "quit"){
        return std::shared_ptr<State>{new QuitState};
    }
    else if (command == "show"){
        return std::shared_ptr<State>{new ShowState};
    }
    else{
        std::cout << "Wrong command. Try again. Type `help` for help.\n";
        return std::shared_ptr<State>{new HomeState};
    }
}
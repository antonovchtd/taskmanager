//
// Created by Anton O. on 11/23/21.
//

#include "State.h"
#include "StateFactory.h"

std::shared_ptr<State> StateFactory::create(const std::string &command) {
    std::string classname;
    if (command == "HomeState"){
        classname = "HomeState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new HomeState};
    }
    else if (command == "add" || command == "AddState"){
        classname = "AddState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new AddState};
    }
    else if (command == "help" || command == "HelpState"){
        classname = "HelpState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new HelpState};
    }
    else if (command == "quit" || command == "QuitState"){
        classname = "QuitState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new QuitState};
    }
    else if (command == "show" || command == "ShowState"){
        classname = "ShowState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new ShowState};
    }
    else if (command == "ReadTitleState"){
        classname = "ReadTitleState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new ReadTitleState};
    }
    else if (command == "ReadPriorityState"){
        classname = "ReadPriorityState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new ReadPriorityState};
    }
    else if (command == "ReadDueDateState"){
        classname = "ReadDueDateState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new ReadDueDateState};
    }
    else if (command == "AddTaskState"){
        classname = "AddTaskState";
        if (steps_.find(classname) == steps_.end())
            steps_[classname] =  std::shared_ptr<State>{new AddTaskState};
    }
    else{
        std::cout << "Wrong command. Try again. Type `help` for help.\n";
        classname = "HomeState";
    }
    return steps_[classname];
}
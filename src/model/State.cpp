//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "State.h"
#include "StateFactory.h"

void State::changeState(const std::shared_ptr<Context> &c, const std::shared_ptr<State> &s) {
    c->changeState(s);
}

void State::print(const std::string &line){
    std::cout << line;
}

std::string State::read(const std::string &prompt) {
    State::print(prompt);
    std::string input;
    getline(std::cin, input);
    return input;
}

void HomeState::execute(Context &c, StateFactory &f) {
    std::string command = State::read(" > ");
    c.changeState(f.create(command));
}

void HelpState::execute(Context &c, StateFactory &f) {
    std::ifstream file("../src/model/help.txt");
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            State::print(line + "\n");
        file.close();
    }
    c.changeState(f.create("HomeState"));
}

void QuitState::execute(Context &c, StateFactory &f) {
    c.changeState(nullptr);
}

void AddState::execute(Context &c, StateFactory &f) {
    State::print("[Add Task]\n");
    c.changeState(f.create("ReadTitleState"));
}

void ReadTitleState::execute(Context &c, StateFactory &f) {
    std::string title;
    while (true){
        title = State::read("    Title > ");
        if (title.empty())
            State::print("Title cannot be empty!\n");
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
        p = State::read("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ");
        pint = std::atoi(p.c_str());
        if (pint >= 0 && pint <= 3)
            break;
        else
            State::print("    Wrong priority option. Try again.\n");
    }
    c.setPriority(static_cast<Task::Priority>(pint));
    c.changeState(f.create("ReadDueDateState"));
}

void ReadDueDateState::execute(Context &c, StateFactory &f) {
    std::optional<time_t> due_date;
    while (true) {
        std::string datestring = State::read("    Due > ");
        due_date = ReadDueDateState::stringToTime(datestring);
        if (due_date)
            break;
        else
            State::print("    Wrong date format. Try again.\n");
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

std::optional<time_t> ReadDueDateState::stringToTime(std::string datestring) {
    std::smatch matches;
    if (std::regex_search(datestring, matches, std::regex(R"(in (\d+:)?(\d+):(\d+))"))){
        return time(nullptr) + std::atoi(matches.str(1).c_str())*24*3600
               + std::atoi(matches.str(2).c_str())*3600
               + std::atoi(matches.str(3).c_str())*60;
    }
    else if (std::regex_search(datestring, matches, std::regex(R"((\d+)/(\d+)(/(\d+))?( (\d+):(\d+))?)"))){
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = localtime(&rawtime);
        timeinfo->tm_mday = std::atoi(matches.str(1).c_str());
        timeinfo->tm_mon = std::atoi(matches.str(2).c_str())-1;
        if (!matches.str(3).empty()){
            int year = std::atoi(matches.str(4).c_str());
            if (year > 1900)
                year -= 1900;
            else
                year += 100;
            timeinfo->tm_year = year;
        }
        if (!matches.str(5).empty()) {
            timeinfo->tm_hour = std::atoi(matches.str(6).c_str());
            timeinfo->tm_min = std::atoi(matches.str(7).c_str());
        }
        else
        {
            timeinfo->tm_hour = 0;
            timeinfo->tm_min = 0;
        }
        timeinfo->tm_sec = 0;
        return mktime(timeinfo);
    }
    else
        return std::nullopt;
}

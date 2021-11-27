//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "Step.h"
#include "StepFactory.h"
#include "Machine.h"

void Step::print(const std::string &line){
    std::cout << line;
}

std::string Step::read(const std::string &prompt) {
    Step::print(prompt);
    std::string input;
    getline(std::cin, input);
    return input;
}

void HomeStep::execute(Context &c, StepFactory &f) {
    std::string command = Step::read(" > ");
    c.changeStep(f.create(command));
}

void HelpStep::execute(Context &c, StepFactory &f) {
    std::ifstream file("../src/model/help.txt");
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            Step::print(line + "\n");
        file.close();
    }
    c.changeStep(f.nextStep(*this));
}

void QuitStep::execute(Context &c, StepFactory &f) {
    c.changeStep(f.nextStep(*this));
}

void AddStep::execute(Context &c, StepFactory &f) {
    Step::print("[Add Task]\n");
    Machine wizard(c);
    c = wizard.run(StepFactory::State::READTITLE);
    c.changeStep(f.nextStep(*this));
}


void ReadIDStep::execute(Context &c, StepFactory &f) {
    std::string input;
    while (true){
        input = Step::read("   Task ID (`show` to show tasks) > ");
        if (input == "show") {
            ShowStep shs;
            shs.execute(c, f); // Note: changes step_ of c, but does not matter
        }
        else if (auto id = TaskID((unsigned int) std::stoi(input)); c.man_->Validate(id)){
            c.setID(id);
            break;
        }
        else
            Step::print("   Bad ID. Try again.\n");

    }
    c.changeStep(f.nextStep(*this));
}

void EditStep::execute(Context &c, StepFactory &f) {
    Step::print("[Edit Task]\n");
    Machine wizard(c);
    c = wizard.run(StepFactory::State::READID);
    c.changeStep(f.nextStep(*this));
}

void SubtaskStep::execute(Context &c, StepFactory &f) {
    Step::print("[Add Subtask]\n");
    Machine wizard(c);
    c = wizard.run(StepFactory::State::READID);
    c.changeStep(f.nextStep(*this));
}

void ReadTitleStep::execute(Context &c, StepFactory &f) {
    std::string title;
    while (true){
        title = Step::read("    Title > ");
        if (title.empty())
            Step::print("Title cannot be empty!\n");
        else
            break;
    }
    c.setTitle(title);
    c.changeStep(f.nextStep(*this));
}

void ReadPriorityStep::execute(Context &c, StepFactory &f) {
    std::string p;
    int pint;
    while (true){
        p = Step::read("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > ");
        pint = p.empty() ? 0 : std::stoi(p);
        if (pint >= 0 && pint <= 3)
            break;
        else
            Step::print("    Wrong priority option. Try again.\n");
    }
    c.setPriority(static_cast<Task::Priority>(pint));
    c.changeStep(f.nextStep(*this));
}

void ReadDueDateStep::execute(Context &c, StepFactory &f) {
    std::optional<time_t> due_date;
    while (true) {
        std::string datestring = Step::read("    Due > ");
        due_date = ReadDueDateStep::stringToTime(datestring);
        if (due_date)
            break;
        else
            Step::print("    Wrong date format. Try again.\n");
    }
    c.setDueDate(due_date.value());
    c.changeStep(f.nextStep(*this));
}

void AddTaskStep::execute(Context &c, StepFactory &f) {
    c.man_->Add(Task::Create(c.data()));
    c.changeStep(f.nextStep(*this));
}

void EditTaskStep::execute(Context &c, StepFactory &f) {
    c.man_->Edit(c.id().value(), Task::Create(c.data()));
    c.changeStep(f.nextStep(*this));
}

void AddSubtaskStep::execute(Context &c, StepFactory &f) {
    c.man_->Add(Task::Create(c.data()), c.id().value());
    c.changeStep(f.nextStep(*this));
}

void ShowStep::execute(Context &c, StepFactory &f) {
    c.man_->Show(std::cout);
    c.changeStep(f.nextStep(*this));
}

std::optional<time_t> ReadDueDateStep::stringToTime(const std::string &datestring) {
    std::smatch matches;
    if (std::regex_search(datestring, matches, std::regex(R"(in (\d+:)?(\d+):(\d+))"))){
        return time(nullptr) + std::stoi(matches.str(1))*24*3600
               + std::stoi(matches.str(2))*3600
               + std::stoi(matches.str(3))*60;
    }
    else if (std::regex_search(datestring, matches, std::regex(R"((\d+)/(\d+)(/(\d+))?( (\d+):(\d+))?)"))){
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = localtime(&rawtime);
        timeinfo->tm_mday = std::stoi(matches.str(1));
        timeinfo->tm_mon = std::stoi(matches.str(2))-1;
        if (!matches.str(3).empty()){
            int year = std::stoi(matches.str(4));
            if (year > 1900)
                year -= 1900;
            else
                year += 100;
            timeinfo->tm_year = year;
        }
        if (!matches.str(5).empty()) {
            timeinfo->tm_hour = std::stoi(matches.str(6));
            timeinfo->tm_min = std::stoi(matches.str(7));
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

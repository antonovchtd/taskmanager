//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "Step.h"
#include "Factory.h"
#include "Machine.h"
#include "../model/utils.h"

void Step::print(std::ostream &os, const std::string &line) {
    os << line;
}

void Step::printFromFile(std::ostream &os, const std::string &fname) {
    std::ifstream file(fname);
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            Step::print(os, line + "\n");
        file.close();
    }
}

std::string Step::read(std::istream &is, std::ostream &os, const std::string &prompt) {
    Step::print(os, prompt);
    std::string input;
    getline(is, input);
    return input;
}

std::shared_ptr<Action> HomeStep::execute(Context &c, Factory &f) {
    std::stringstream ss{Step::read(*c.in(), *c.out(), " > ")};
    std::string command, arg;
    ss >> command >> arg;
    c.setStep(f.create(c, command));
    c.setArg(arg);
    return c.getStep()->getValidateArgAction(f);
}

void HomeStep::process(Context &c, Factory &f) {
    if (c.id().has_value()) {
        if (!c.id()->isValid())
            Step::print(*c.out(), "Invalid ID. Try again.\n");
    } else {
        Step::print(*c.out(), "This function takes no argument.\n");
    }
}

std::shared_ptr<Action> HomeStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

std::shared_ptr<Action> HelpStep::execute(Context &c, Factory &f) {
    Step::printFromFile(*c.out(), "../src/model/help.txt");
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void HelpStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> HelpStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

std::shared_ptr<Action> AddStep::execute(Context &c, Factory &f) {
    Step::print(*c.out(), "[Add Task]\n");
    Machine wizard(c, Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void AddStep::process(Context &c, Factory &f) {
    Step::print(*c.out(), "Added Task with ID " + c.id().value().to_string() + ".\n");
    c.resetTaskData();
}

std::shared_ptr<Action> AddStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

bool ReadTaskDataStep::validateTitle(const Context &c, const std::string &title) {
    if (title.empty()) {
        Step::print(*c.out(), "Title cannot be empty!\n");
        return false;
    }
    return true;
}

std::optional<Task::Priority> ReadTaskDataStep::stringToPriority(const Context &c, const std::string &priority) {
    int pint = priority.empty() ? 0 : std::stoi(priority);
    if (pint >= 0 && pint <= 3)
        return static_cast<Task::Priority>(pint);
    else {
        Step::print(*c.out(), "    Wrong priority option. Try again.\n");
        return std::nullopt;
    }
}

std::optional<time_t> ReadTaskDataStep::stringToTime(const Context &c, const std::string &datestring) {
    std::smatch matches;
    if (std::regex_search(datestring, matches,
                          std::regex(R"(in (\d+:)?(\d+):(\d+))"))) {
        return time(nullptr) + std::stoi(matches.str(1))*24*3600
               + std::stoi(matches.str(2))*3600
               + std::stoi(matches.str(3))*60;
    } else if (std::regex_search(datestring, matches,
                                 std::regex(R"((\d+)[./](\d+)([./](\d+))?( (\d+):(\d+))?)"))){
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = localtime(&rawtime);
        timeinfo->tm_mday = std::stoi(matches.str(1));
        timeinfo->tm_mon = std::stoi(matches.str(2))-1;
        if (!matches.str(3).empty()) {
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
        } else {
            timeinfo->tm_hour = 0;
            timeinfo->tm_min = 0;
        }
        timeinfo->tm_sec = 0;
        return mktime(timeinfo);
    } else {
        Step::print(*c.out(), "    Wrong date format. Try again.\n");
        return std::nullopt;
    }
}

std::shared_ptr<Action> ReadTaskDataStep::execute(Context &c, Factory &f) {
    std::string title;
    do {
        title = Step::read(*c.in(), *c.out(), "    Title > ");
    } while (!validateTitle(c, title));
    c.setTitle(title);

    std::optional<Task::Priority> priority{std::nullopt};
    while (!priority) {
        priority = stringToPriority(c, Step::read(*c.in(), *c.out(),
                                                  "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > "));
    }
    c.setPriority(priority.value());

    std::optional<time_t> due_date;
    while (!due_date) {
        due_date = ReadTaskDataStep::stringToTime(c, Step::read(*c.in(), *c.out(),
                                                                "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > "));
    }
    c.setDueDate(due_date.value());

    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void ReadTaskDataStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> ReadTaskDataStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

std::shared_ptr<Action> EditStep::execute(Context &c, Factory &f) {
    Step::print(*c.out(), "[Edit Task]\n");
    Machine wizard(c, Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void EditStep::process(Context &c, Factory &f) {
    Step::print(*c.out(), "Edited Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> EditStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> SubtaskStep::execute(Context &c, Factory &f) {
    Step::print(*c.out(), "[Add Subtask]\n");
    Machine wizard(c, Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void SubtaskStep::process(Context &c, Factory &f) {
    Step::print(*c.out(), "Added Subtask with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> SubtaskStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> QuitStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void QuitStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> QuitStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

std::shared_ptr<Action> ShowStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void ShowStep::process(Context &c, Factory &f) {
    *c.out() << c.getTasks();
}

std::shared_ptr<Action> ShowStep::getValidateArgAction(Factory &f) {
    return f.getValidateLabelArgAction();
}

std::shared_ptr<Action> CompleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void CompleteStep::process(Context &c, Factory &f) {
    Step::print(*c.out(), "Marked Task with ID " + c.id().value().to_string() + " as completed.\n");
}

std::shared_ptr<Action> CompleteStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> DeleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void DeleteStep::process(Context &c, Factory &f) {
    Step::print(*c.out(), "Deleted Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> DeleteStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> LabelStep::execute(Context &c, Factory &f) {
    c.setLabel(Step::read(*c.in(), *c.out(), "[Add Label]\n    >> "));
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void LabelStep::process(Context &c, Factory &f) {
    Step::print(*c.out(), "Added label to Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> LabelStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}
//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "Step.h"
#include "Factory.h"
#include "Machine.h"
#include "../model/utils.h"

void Step::print(const std::string &line){
    std::cout << line;
}

void Step::printFromFile(const std::string &fname){
    std::ifstream file(fname);
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            Step::print(line + "\n");
        file.close();
    };
}

std::string Step::read(const std::string &prompt) {
    Step::print(prompt);
    std::string input;
    getline(std::cin, input);
    return input;
}

std::shared_ptr<Action> HomeStep::execute(Context &c, Factory &f) {
    std::stringstream ss{Step::read(" > ")};
    std::string command, id;
    ss >> command >> id;
    c.setStep(f.create(command));
    c.setID(TaskID::Create(id));
    return c.getStep()->getValidateIDAction(f);
}

void HomeStep::process(Context &c, Factory &f) {
    if (c.id().has_value()) {
        if (!c.id()->isValid())
            Step::print("Invalid ID. Try again.\n");
    } else {
        Step::print("This function takes no ID value.\n");
    }
}

std::shared_ptr<Action> HomeStep::getValidateIDAction(Factory &f) {
    return f.getValidateNoIDAction();
}

std::shared_ptr<Action> HelpStep::execute(Context &c, Factory &f) {
    Step::printFromFile("../src/model/help.txt");
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void HelpStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> HelpStep::getValidateIDAction(Factory &f) {
    return f.getValidateNoIDAction();
}

std::shared_ptr<Action> AddStep::execute(Context &c, Factory &f) {
    Step::print("[Add Task]\n");
    Machine wizard(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void AddStep::process(Context &c, Factory &f) {
    Step::print("Added Task with ID " + c.id().value().to_string() + ".\n");
    c.resetTaskData();
}

std::shared_ptr<Action> AddStep::getValidateIDAction(Factory &f) {
    return f.getValidateNoIDAction();
}

bool ReadTaskDataStep::validateTitle(const std::string &title) {
    if (title.empty()) {
        Step::print("Title cannot be empty!\n");
        return false;
    }
    return true;
}

std::optional<Task::Priority> ReadTaskDataStep::stringToPriority(const std::string &priority) {
    int pint = priority.empty() ? 0 : std::stoi(priority);
    if (pint >= 0 && pint <= 3)
        return static_cast<Task::Priority>(pint);
    else {
        Step::print("    Wrong priority option. Try again.\n");
        return std::nullopt;
    }
}

std::optional<time_t> ReadTaskDataStep::stringToTime(const std::string &datestring) {
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
        Step::print("    Wrong date format. Try again.\n");
        return std::nullopt;
    }
}

std::shared_ptr<Action> ReadTaskDataStep::execute(Context &c, Factory &f) {
    std::string title;
    do {
        title = Step::read("    Title > ");
    } while (!validateTitle(title));
    c.setTitle(title);

    std::optional<Task::Priority> priority{std::nullopt};
    while (!priority) {
        priority = stringToPriority(Step::read("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > "));
    };
    c.setPriority(priority.value());

    std::optional<time_t> due_date;
    while (!due_date) {
        due_date = ReadTaskDataStep::stringToTime(Step::read("    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > "));
    }
    c.setDueDate(due_date.value());

    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void ReadTaskDataStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> ReadTaskDataStep::getValidateIDAction(Factory &f) {
    return f.getValidateNoIDAction();
}

std::shared_ptr<Action> EditStep::execute(Context &c, Factory &f) {
    Step::print("[Edit Task]\n");
    Machine wizard(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void EditStep::process(Context &c, Factory &f) {
    Step::print("Edited Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> EditStep::getValidateIDAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> SubtaskStep::execute(Context &c, Factory &f) {
    Step::print("[Add Subtask]\n");
    Machine wizard(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void SubtaskStep::process(Context &c, Factory &f) {
    Step::print("Added Subtask with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> SubtaskStep::getValidateIDAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> QuitStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void QuitStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> QuitStep::getValidateIDAction(Factory &f) {
    return f.getValidateNoIDAction();
}

std::shared_ptr<Action> ShowStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void ShowStep::process(Context &c, Factory &f) {
    std::cout << c.getTasks();
}

std::shared_ptr<Action> ShowStep::getValidateIDAction(Factory &f) {
    return f.getValidateNoIDAction();
}

std::shared_ptr<Action> CompleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void CompleteStep::process(Context &c, Factory &f) {
    Step::print("Marked Task with ID " + c.id().value().to_string() + " as completed.\n");
}

std::shared_ptr<Action> CompleteStep::getValidateIDAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> DeleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void DeleteStep::process(Context &c, Factory &f) {
    Step::print("Deleted Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> DeleteStep::getValidateIDAction(Factory &f) {
    return f.getValidateIDAction();
}

std::shared_ptr<Action> LabelStep::execute(Context &c, Factory &f) {
    c.setLabel(Step::read("[Add Label]\n    >> "));
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void LabelStep::process(Context &c, Factory &f) {
    Step::print("Added label to Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> LabelStep::getValidateIDAction(Factory &f) {
    return f.getValidateIDAction();
}
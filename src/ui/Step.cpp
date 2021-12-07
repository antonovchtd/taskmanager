//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "Step.h"
#include "Factory.h"
#include "Machine.h"

Step::Step(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
            reader_(std::move(reader)), printer_(std::move(printer)) {

}

std::shared_ptr<AbstractReader> Step::reader() const {
    return reader_;
}

std::shared_ptr<AbstractPrinter> Step::printer() const {
    return printer_;
}

HomeStep::HomeStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> HomeStep::execute(Context &c, Factory &f) {
    std::stringstream ss{reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    c.setStep(f.create(command));
    c.setArg(arg);
    return c.getStep()->getValidateArgAction(f);
}

void HomeStep::process(Context &c, Factory &f) {
    if (c.id().has_value()) {
        if (!c.id()->isValid())
            printer()->print("Invalid ID. Try again.\n");
    } else {
        printer()->print("This function takes no argument.\n");
    }
}

std::shared_ptr<Action> HomeStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

HelpStep::HelpStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> HelpStep::execute(Context &c, Factory &f) {
    FileReader fr("../src/model/help.txt");
    printer()->print(fr.read(""));
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void HelpStep::process(Context &c, Factory &f) {
    // do nothing
}

std::shared_ptr<Action> HelpStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

AddStep::AddStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> AddStep::execute(Context &c, Factory &f) {
    printer()->print("[Add Task]\n");
    Machine wizard(f, Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void AddStep::process(Context &c, Factory &f) {
    printer()->print("Added Task with ID " + c.id().value().to_string() + ".\n");
    c.resetTaskData();
}

std::shared_ptr<Action> AddStep::getValidateArgAction(Factory &f) {
    return f.getValidateNoArgAction();
}

ReadTaskDataStep::ReadTaskDataStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

bool ReadTaskDataStep::validateTitle(const Context &c, const std::string &title) {
    if (title.empty()) {
        printer()->print("Title cannot be empty!\n");
        return false;
    }
    return true;
}

std::optional<Task::Priority> ReadTaskDataStep::stringToPriority(const Context &c, const std::string &priority) {
    int pint = priority.empty() ? 0 : std::stoi(priority);
    if (pint >= 0 && pint <= 3)
        return static_cast<Task::Priority>(pint);
    else {
        printer()->print("    Wrong priority option. Try again.\n");
        return std::nullopt;
    }
}

std::optional<time_t> ReadTaskDataStep::stringToTime(const Context &c, const std::string &datestring) {
    std::smatch matches;
    if (std::regex_search(datestring, matches,
                          std::regex(R"(in (\d+:)?(\d+):(\d+))"))) {
        return time(nullptr) + std::stoi("0" + matches.str(1))*24*3600
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
        printer()->print("    Wrong date format. Try again.\n");
        return std::nullopt;
    }
}

std::shared_ptr<Action> ReadTaskDataStep::execute(Context &c, Factory &f) {
    std::string title;
    do {
        title = reader()->read("    Title > ");
    } while (!validateTitle(c, title));
    c.setTitle(title);

    std::optional<Task::Priority> priority{std::nullopt};
    while (!priority) {
        priority = stringToPriority(c, reader()->read("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > "));
    }
    c.setPriority(priority.value());

    std::optional<time_t> due_date;
    while (!due_date) {
        due_date = ReadTaskDataStep::stringToTime(c, reader()->read("    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > "));
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

EditStep::EditStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> EditStep::execute(Context &c, Factory &f) {
    printer()->print("[Edit Task]\n");
    Machine wizard(f, Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void EditStep::process(Context &c, Factory &f) {
    printer()->print("Edited Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> EditStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

SubtaskStep::SubtaskStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> SubtaskStep::execute(Context &c, Factory &f) {
    printer()->print("[Add Subtask]\n");
    Machine wizard(f, Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void SubtaskStep::process(Context &c, Factory &f) {
    printer()->print("Added Subtask with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> SubtaskStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

QuitStep::QuitStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
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

ShowStep::ShowStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> ShowStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void ShowStep::recursivePrint(const std::pair<TaskID, std::pair<Task, Node>> &kv,
                              const Context &c,
                              const std::string &prefix) {
    printer()->print(prefix + kv.first.to_string() +
                     " – " + kv.second.first.to_string());

    if (!kv.second.second.label().empty())
        printer()->print(" :L" + kv.second.second.label());

    printer()->print("\n");
    for (const auto &id : kv.second.second.children()) {
        auto ch = std::make_pair(id, c.getTasks().at(id));
        recursivePrint(ch, c, prefix + "    ");
    }
}

void ShowStep::process(Context &c, Factory &f) {
    auto tasks = c.getTasks();
    for (const auto &kv : tasks) {
        if (!kv.second.second.parent().isValid())
            ShowStep::recursivePrint(kv, c, "");
    }
}

std::shared_ptr<Action> ShowStep::getValidateArgAction(Factory &f) {
    return f.getValidateLabelArgAction();
}

CompleteStep::CompleteStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> CompleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void CompleteStep::process(Context &c, Factory &f) {
    printer()->print("Marked Task with ID " + c.id().value().to_string() + " as completed.\n");
}

std::shared_ptr<Action> CompleteStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

DeleteStep::DeleteStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> DeleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void DeleteStep::process(Context &c, Factory &f) {
    printer()->print("Deleted Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> DeleteStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

ConfirmDeleteStep::ConfirmDeleteStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> ConfirmDeleteStep::execute(Context &c, Factory &f) {
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void ConfirmDeleteStep::process(Context &c, Factory &f) {
    std::string reply;
    if (c.askConfirmation())
        while (true){
            reply = reader()->read("Task " + c.id().value().to_string() +
                                   " has subtasks. Confirm to delete all. [Y]/N > ");
            if (reply.empty() || reply == "Y" || reply == "y") {
                c.setStep(f.getDeleteStep());
                c.setOldStep(f.getHomeStep());
                break;
            } else if (reply == "N" || reply == "n") {
                c.setStep(f.getHomeStep());
                c.setOldStep(f.getHomeStep());
                break;
            } else {
                printer()->print("Wrong option. Type Y or N.\n");
            }
        }
    else {
        c.setStep(f.getDeleteStep());
        c.setOldStep(f.getHomeStep());
    }

}

std::shared_ptr<Action> ConfirmDeleteStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}

LabelStep::LabelStep(std::shared_ptr<AbstractReader> reader, std::shared_ptr<AbstractPrinter> printer) :
        Step(std::move(reader), std::move(printer)) {
}

std::shared_ptr<Action> LabelStep::execute(Context &c, Factory &f) {
    c.setLabel(reader()->read("[Add Label]\n    >> "));
    c.setStep(f.nextStep(*this));
    return f.getAction(*this);
}

void LabelStep::process(Context &c, Factory &f) {
    printer()->print("Added label to Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> LabelStep::getValidateArgAction(Factory &f) {
    return f.getValidateIDAction();
}
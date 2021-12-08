//
// Created by Anton O. on 11/22/21.
//

#include "Context.h"
#include "Step.h"
#include "Factory.h"
#include "Machine.h"

Step::Step(const std::shared_ptr<AbstractReader> &reader,
           const std::shared_ptr<AbstractPrinter> &printer,
           const std::shared_ptr<Factory> &factory) :
            reader_(reader),
            printer_(printer),
            factory_(factory) {

}

std::shared_ptr<AbstractReader> Step::reader() const {
    return reader_;
}

std::shared_ptr<AbstractPrinter> Step::printer() const {
    return printer_;
}

std::shared_ptr<Factory> Step::factory() const {
    return factory_;
}

HomeStep::HomeStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer,
                   const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> HomeStep::execute(Context &c) {
    std::stringstream ss{reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    c.setStep(factory()->createStep(command));
    c.setArg(arg);
    return c.getStep()->getValidateArgAction();
}

void HomeStep::process(Context &c) {
    if (c.id().has_value()) {
        if (!c.id()->isValidOrNull())
            printer()->print("Invalid ID. Try again.\n");
    } else {
        printer()->print("This function takes no argument.\n");
    }
}

std::shared_ptr<Action> HomeStep::getValidateArgAction() {
    return factory()->getValidateNoArgAction();
}

HelpStep::HelpStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer,
                   const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> HelpStep::execute(Context &c) {
    FileReader fr("../src/model/help.txt");
    printer()->print(fr.read(""));
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void HelpStep::process(Context &c) {
    // do nothing
}

std::shared_ptr<Action> HelpStep::getValidateArgAction() {
    return factory()->getValidateNoArgAction();
}

AddStep::AddStep(const std::shared_ptr<AbstractReader> &reader,
                 const std::shared_ptr<AbstractPrinter> &printer,
                 const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> AddStep::execute(Context &c) {
    printer()->print("[Add Task]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void AddStep::process(Context &c) {
    printer()->print("Added Task with ID " + c.id().value().to_string() + ".\n");
    c.resetTaskData();
}

std::shared_ptr<Action> AddStep::getValidateArgAction() {
    return factory()->getValidateNoArgAction();
}

ReadTaskDataStep::ReadTaskDataStep(const std::shared_ptr<AbstractReader> &reader,
                                   const std::shared_ptr<AbstractPrinter> &printer,
                                   const std::shared_ptr<Factory> &factory) :
                    Step(reader, printer, factory) {
}

bool ReadTaskDataStep::validateTitle(const Context &c, const std::string &title) {
    if (title.empty()) {
        printer()->print("Title cannot be empty!\n");
        return false;
    }
    return true;
}

std::optional<Task::Priority> ReadTaskDataStep::stringToPriority(const Context &c, const std::string &priority) {
    int pint;
    try {
        pint = priority.empty() ? 0 : std::stoi(priority);
    } catch (const std::invalid_argument &) {
        pint = -1;
    }
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

std::shared_ptr<Action> ReadTaskDataStep::execute(Context &c) {
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

    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void ReadTaskDataStep::process(Context &c) {
    // do nothing
}

std::shared_ptr<Action> ReadTaskDataStep::getValidateArgAction() {
    return factory()->getValidateNoArgAction();
}

EditStep::EditStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer,
                   const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> EditStep::execute(Context &c) {
    printer()->print("[Edit Task]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void EditStep::process(Context &c) {
    printer()->print("Edited Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> EditStep::getValidateArgAction() {
    return factory()->getValidateIDAction();
}

SubtaskStep::SubtaskStep(const std::shared_ptr<AbstractReader> &reader,
                         const std::shared_ptr<AbstractPrinter> &printer,
                         const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> SubtaskStep::execute(Context &c) {
    printer()->print("[Add Subtask]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setData(input_context.data());
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void SubtaskStep::process(Context &c) {
    printer()->print("Added Subtask with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> SubtaskStep::getValidateArgAction() {
    return factory()->getValidateIDAction();
}

QuitStep::QuitStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer,
                   const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> QuitStep::execute(Context &c) {
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void QuitStep::process(Context &c) {
    // do nothing
}

std::shared_ptr<Action> QuitStep::getValidateArgAction() {
    return factory()->getValidateNoArgAction();
}

ShowStep::ShowStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer,
                   const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> ShowStep::execute(Context &c) {
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void ShowStep::recursivePrint(const std::pair<TaskID, std::pair<Task, Node>> &kv,
                              const Context &c,
                              const std::string &prefix) {
    printer()->print(prefix + kv.first.to_string() +
                     " – " + kv.second.first.to_string());

    if (!kv.second.second.label().empty())
        printer()->print(" :L " + kv.second.second.label());

    printer()->print("\n");
    for (const auto &id : kv.second.second.children()) {
        auto ch = std::make_pair(id, c.getTasks().at(id));
        recursivePrint(ch, c, prefix + "    ");
    }
}

void ShowStep::process(Context &c) {
    auto tasks = c.getTasks();
    for (const auto &kv : tasks) {
        if (!kv.second.second.parent().isValid())
            ShowStep::recursivePrint(kv, c, "");
    }
}

std::shared_ptr<Action> ShowStep::getValidateArgAction() {
    return factory()->getValidateLabelArgAction();
}

CompleteStep::CompleteStep(const std::shared_ptr<AbstractReader> &reader,
                           const std::shared_ptr<AbstractPrinter> &printer,
                           const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> CompleteStep::execute(Context &c) {
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void CompleteStep::process(Context &c) {
    printer()->print("Marked Task with ID " + c.id().value().to_string() + " as completed.\n");
}

std::shared_ptr<Action> CompleteStep::getValidateArgAction() {
    return factory()->getValidateIDAction();
}

DeleteStep::DeleteStep(const std::shared_ptr<AbstractReader> &reader,
                       const std::shared_ptr<AbstractPrinter> &printer,
                       const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> DeleteStep::execute(Context &c) {
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void DeleteStep::process(Context &c) {
    printer()->print("Deleted Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> DeleteStep::getValidateArgAction() {
    return factory()->getValidateIDAction();
}

ConfirmDeleteStep::ConfirmDeleteStep(const std::shared_ptr<AbstractReader> &reader,
                                     const std::shared_ptr<AbstractPrinter> &printer,
                                     const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> ConfirmDeleteStep::execute(Context &c) {
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void ConfirmDeleteStep::process(Context &c) {
    std::string reply;
    if (c.askConfirmation())
        while (true){
            reply = reader()->read("Task " + c.id().value().to_string() +
                                   " has subtasks. Confirm to delete all. [Y]/N > ");
            if (reply.empty() || reply == "Y" || reply == "y") {
                c.setStep(factory()->getDeleteStep());
                c.setOldStep(factory()->getHomeStep());
                break;
            } else if (reply == "N" || reply == "n") {
                c.setStep(factory()->getHomeStep());
                c.setOldStep(factory()->getHomeStep());
                break;
            } else {
                printer()->print("Wrong option. Type Y or N.\n");
            }
        }
    else {
        c.setStep(factory()->getDeleteStep());
        c.setOldStep(factory()->getHomeStep());
    }

}

std::shared_ptr<Action> ConfirmDeleteStep::getValidateArgAction() {
    return factory()->getValidateIDAction();
}

LabelStep::LabelStep(const std::shared_ptr<AbstractReader> &reader,
                     const std::shared_ptr<AbstractPrinter> &printer,
                     const std::shared_ptr<Factory> &factory) :
        Step(reader, printer, factory) {
}

std::shared_ptr<Action> LabelStep::execute(Context &c) {
    c.setLabel(reader()->read("[Add Label]\n    >> "));
    c.setStep(factory()->nextStep(*this));
    return factory()->getAction(*this);
}

void LabelStep::process(Context &c) {
    printer()->print("Added label to Task with ID " + c.id().value().to_string() + ".\n");
}

std::shared_ptr<Action> LabelStep::getValidateArgAction() {
    return factory()->getValidateIDAction();
}
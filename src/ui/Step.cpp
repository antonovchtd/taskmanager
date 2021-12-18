//
// Created by Anton O. on 11/22/21.
//

#include "Step.h"
#include "Machine.h"

Step::Step(const std::shared_ptr<Factory> &factory) : factory_(factory) {

}

std::shared_ptr<Factory> Step::factory() const {
    return factory_;
}

std::shared_ptr<Action> HomeStep::execute(Context &c) {
    std::stringstream ss{factory()->reader()->read(" > ")};
    std::string arg;
    ss >> command_ >> arg;
    return getValidateArgAction(arg);
}

void HomeStep::process(Context &c) {
    if (c.id().has_value()) {
        if (c.id()->has_is_invalid() && c.id()->is_invalid()) {
            factory()->printer()->print("Invalid ID. Try again.\n");
            command_ = "";
        }
    } else {
        factory()->printer()->print("This function takes no argument.\n");
        command_ = "";
    }
    c.setStep(factory()->createStep(command_));
}

std::shared_ptr<Action> HomeStep::getValidateArgAction(const std::string &arg) {
    std::shared_ptr<Action> action;
    if (command_ == "edit" || command_ == "subtask" ||
        command_ == "delete" || command_ == "complete" ||
        command_ == "label") {
        action = factory()->lazyInitAction(Factory::ActionLabel::VALIDATEID);
    } else if (command_ == "show" || command_ == "save" || command_ == "load") {
        action = factory()->lazyInitAction(Factory::ActionLabel::VALIDATELABEL);
    } else {
        action = factory()->lazyInitAction(Factory::ActionLabel::VALIDATENOID);
    }
    action->setActionData(Action::Data{arg});
    return action;
}

std::shared_ptr<Action> HelpStep::execute(Context &c) {
    FileReader fr("../src/model/help.txt");
    factory()->printer()->print(fr.read(""));
    return ActionGetter::getAction(*this, factory());
}

void HelpStep::process(Context &c) {
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> AddStep::execute(Context &c) {
    factory()->printer()->print("[Add Task]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());
    return ActionGetter::getAction(*this, factory());
}

void AddStep::process(Context &c) {
    factory()->printer()->print("Added Task with ID " + std::to_string(c.id()->num()) + ".\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

bool ReadTaskDataStep::validateTitle(const Context &c, const std::string &title) {
    if (title.empty()) {
        factory()->printer()->print("    Title cannot be empty!\n");
        return false;
    }
    return true;
}

std::optional<ProtoTask::Task::Priority> ReadTaskDataStep::stringToPriority(const Context &c, const std::string &priority) {
    int pint;
    try {
        pint = priority.empty() ? 0 : std::stoi(priority);
    } catch (const std::invalid_argument &) {
        pint = -1;
    }
    if (pint >= 0 && pint <= 3)
        return static_cast<ProtoTask::Task::Priority>(pint);
    else {
        factory()->printer()->print("    Wrong priority option. Try again.\n");
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
        factory()->printer()->print("    Wrong date format. Try again.\n");
        return std::nullopt;
    }
}

std::shared_ptr<Action> ReadTaskDataStep::execute(Context &c) {
    std::string title;
    do {
        title = factory()->reader()->read("    Title > ");
    } while (!validateTitle(c, title));
    c.setTitle(title);

    std::optional<ProtoTask::Task::Priority> priority{std::nullopt};
    while (!priority) {
        priority = stringToPriority(c, factory()->reader()->read("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > "));
    }
    c.setPriority(*priority);

    std::optional<time_t> due_date;
    while (!due_date) {
        due_date = ReadTaskDataStep::stringToTime(c, factory()->reader()->read("    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > "));
    }
    c.setDueDate(*due_date);

    return ActionGetter::getAction(*this, factory());
}

void ReadTaskDataStep::process(Context &c) {
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> EditStep::execute(Context &c) {
    factory()->printer()->print("[Edit Task]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());
    return ActionGetter::getAction(*this, factory());
}

void EditStep::process(Context &c) {
    factory()->printer()->print("Edited Task with ID " + std::to_string(c.id()->num()) + ".\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> SubtaskStep::execute(Context &c) {
    factory()->printer()->print("[Add Subtask]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());
    return ActionGetter::getAction(*this, factory());
}

void SubtaskStep::process(Context &c) {
    factory()->printer()->print("Added Subtask with ID " + std::to_string(c.id()->num()) + ".\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> QuitStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void QuitStep::process(Context &c) {
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> ShowStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void ShowStep::recursivePrint(const std::pair<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> &kv,
                              const Context &c,
                              const std::string &prefix) {
    factory()->printer()->print(prefix + std::to_string(kv.first.num()) +
                     " – " + to_string(kv.second.first));

    factory()->printer()->print("\n");
    for (const auto &id : kv.second.second.children()) {
        auto ch = std::make_pair(id, c.tasks().at(id));
        recursivePrint(ch, c, prefix + "    ");
    }
}

void ShowStep::process(Context &c) {
    auto tasks = c.tasks();
    for (const auto &kv : tasks) {
        if (!kv.second.second.parent())
            ShowStep::recursivePrint(kv, c, "");
    }
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> CompleteStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void CompleteStep::process(Context &c) {
    factory()->printer()->print("Marked Task with ID " + std::to_string(c.id()->num()) + " as completed.\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> DeleteStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void DeleteStep::process(Context &c) {
    factory()->printer()->print("Deleted Task with ID " + std::to_string(c.id()->num()) + ".\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> ConfirmDeleteStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void ConfirmDeleteStep::process(Context &c) {
    // proceed to DeleteStep
    c.setStep(StepSwitcher::nextStep(*this, factory()));
    std::string reply;
    auto ch = c.tasks().at(*c.id()).second.children();
    if (!ch.empty())
        while (true){
            reply = factory()->reader()->read("Task " + std::to_string(c.id()->num()) +
                                   " has " + std::to_string(ch.size()) +
                                   " subtask(s). Confirm to delete all. [Y]/N > ");
            if (reply.empty() || reply == "Y" || reply == "y") {
                break;
            } else if (reply == "N" || reply == "n") {
                // disregard nextStep and go to HomeStep
                c.setStep(factory()->lazyInitStep(Factory::State::HOME));
                break;
            } else {
                factory()->printer()->print("Wrong option. Type Y or N.\n");
            }
        }

}

std::shared_ptr<Action> LabelStep::execute(Context &c) {
    std::shared_ptr<Action> action = ActionGetter::getAction(*this, factory());
    std::string label = factory()->reader()->read("[Add Label]\n    >> ");
    action->setActionData(Action::Data{label});
    return action;
}

void LabelStep::process(Context &c) {
    factory()->printer()->print("Added label to Task with ID " + std::to_string(c.id()->num()) + ".\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> SaveStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void SaveStep::process(Context &c) {
    factory()->printer()->print("Saved to file successfully.\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}

std::shared_ptr<Action> LoadStep::execute(Context &c) {
    return ActionGetter::getAction(*this, factory());
}

void LoadStep::process(Context &c) {
    factory()->printer()->print("Loaded from file successfully.\n");
    c.setStep(StepSwitcher::nextStep(*this, factory()));
}
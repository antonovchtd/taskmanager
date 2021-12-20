//
// Created by Anton O. on 11/22/21.
//

#include "Step.h"
#include "Machine.h"
#include "StepSwitcher.h"

Step::Step(const std::shared_ptr<Factory> &factory) : factory_(factory) {

}

std::shared_ptr<Factory> Step::factory() const {
    return factory_;
}

std::shared_ptr<Step> HomeStep::execute(Context &c) {
    std::stringstream ss{factory()->reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    
    std::shared_ptr<Action> action;
    if (command == "edit" || command == "subtask" ||
        command == "delete" || command == "complete" ||
        command == "label") {
        action = factory()->lazyInitAction(Factory::ActionLabel::VALIDATEID);
    } else if (command == "show" || command == "save" ||
               command == "load" || command.empty()) {
        action = factory()->lazyInitAction(Factory::ActionLabel::VALIDATELABEL);
    } else {
        action = factory()->lazyInitAction(Factory::ActionLabel::VALIDATENOARG);
    }
    action->setActionData(Action::Data{arg});
    ActionResult result = action->make(c);

    if (result)
        c.setID(result.id);
    else {
        factory()->printer()->print(result.message());
        command = "";
    }
    return factory()->createStep(command);
}

std::shared_ptr<Step> HelpStep::execute(Context &c) {
    FileReader fr("../src/model/help.txt");
    factory()->printer()->print(fr.read(""));
    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> AddStep::execute(Context &c) {
    factory()->printer()->print("[Add Task]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    auto action = factory()->lazyInitAction(Factory::ActionLabel::ADDTASK);
    ActionResult result = action->make(c);

    if (result) {
            factory()->printer()->print("Added Task with ID " + std::to_string(result.id->num()) + ".\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }
    return StepSwitcher::nextStep(*this, factory());
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

std::shared_ptr<Step> ReadTaskDataStep::execute(Context &c) {
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

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> EditStep::execute(Context &c) {
    factory()->printer()->print("[Edit Task]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    auto action = factory()->lazyInitAction(Factory::ActionLabel::EDIT);
    ActionResult result = action->make(c);


    if (result) {
        factory()->printer()->print("Edited Task with ID " + std::to_string(result.id->num()) + ".\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> SubtaskStep::execute(Context &c) {
    factory()->printer()->print("[Add Subtask]\n");
    Machine wizard(factory(), Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());
    auto action = factory()->lazyInitAction(Factory::ActionLabel::ADDSUBTASK);
    ActionResult result = action->make(c);

    if (result) {
        factory()->printer()->print("Added Subtask with ID " + std::to_string(result.id->num()) + ".\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> QuitStep::execute(Context &c) {
    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> ShowStep::execute(Context &c) {
    std::shared_ptr<Action> action = factory()->lazyInitAction(Factory::ActionLabel::SHOW);
    std::shared_ptr<Action> labelAction = factory()->lazyInitAction(Factory::ActionLabel::VALIDATELABEL);
    action->setActionData(labelAction->data());
    ActionResult result = action->make(c);

    auto tasks = c.tasks();
    for (const auto &kv : tasks) {
        if (!kv.second.second.parent())
            ShowStep::recursivePrint(kv, c, "");
    }

    return StepSwitcher::nextStep(*this, factory());
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

std::shared_ptr<Step> CompleteStep::execute(Context &c) {
    auto action = factory()->lazyInitAction(Factory::ActionLabel::COMPLETE);
    ActionResult result = action->make(c);

    if (result) {
        factory()->printer()->print("Marked Task with ID " +
                                    std::to_string(result.id->num()) + " as completed.\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> DeleteStep::execute(Context &c) {
    auto action = factory()->lazyInitAction(Factory::ActionLabel::DELETE);
    ActionResult result = action->make(c);

    if (result) {
        factory()->printer()->print("Deleted Task with ID " +
                                     std::to_string(result.id->num()) + ".\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> ConfirmDeleteStep::execute(Context &c) {
    auto action = factory()->lazyInitAction(Factory::ActionLabel::CONFIRMDELETE);
    ActionResult result = action->make(c);

    if (result) {
        auto ch = c.tasks().at(*result.id).second.children();
        if (!ch.empty()) {
            while (true) {
                std::string reply = factory()->reader()->read("Task " + std::to_string(c.id()->num()) +
                                                              " has " + std::to_string(ch.size()) +
                                                              " subtask(s). Confirm to delete all. [Y]/N > ");
                if (reply.empty() || reply == "Y" || reply == "y") {
                    break;
                } else if (reply == "N" || reply == "n") {
                    // disregard nextStep and go to HomeStep
                    return factory()->lazyInitStep(Factory::State::HOME);
                } else {
                    factory()->printer()->print("Wrong option. Type Y or N.\n");
                }
            }
        }
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> LabelStep::execute(Context &c) {
    std::shared_ptr<Action> action = factory()->lazyInitAction(Factory::ActionLabel::LABEL);
    std::string label;
    while (label.empty())
        label = factory()->reader()->read("[Add Label]\n    >> ");
    action->setActionData(Action::Data{label});
    ActionResult result = action->make(c);

    if (result) {
        factory()->printer()->print("Added label to Task with ID " + std::to_string(result.id->num()) + ".\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> SaveStep::execute(Context &c) {
    std::shared_ptr<Action> action = factory()->lazyInitAction(Factory::ActionLabel::SAVE);
    std::shared_ptr<Action> labelAction = factory()->lazyInitAction(Factory::ActionLabel::VALIDATELABEL);
    action->setActionData(labelAction->data());
    ActionResult result = action->make(c);

    if (result) {
        factory()->printer()->print("Saved to file successfully.\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

std::shared_ptr<Step> LoadStep::execute(Context &c) {
    std::shared_ptr<Action> action = factory()->lazyInitAction(Factory::ActionLabel::LOAD);
    std::shared_ptr<Action> labelAction = factory()->lazyInitAction(Factory::ActionLabel::VALIDATELABEL);
    action->setActionData(labelAction->data());
    ActionResult result = action->make(c);

    if (result) {
        factory()->printer()->print("Loaded from file successfully.\n");
    } else {
        factory()->printer()->print(result.message());
        return factory()->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, factory());
}

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

std::string HomeStep::command() const {
    return command_;
}

template <typename T>
std::shared_ptr<Step> processResult(const T &step,
                                    const ActionResult &result,
                                    const std::string &message) {
    if (result) {
        if (!message.empty()) {
            step.factory()->printer()->print(message);
            if (result.id)
                step.factory()->printer()->print(" (ID: " + std::to_string(result.id->value()) + ")\n");
        }
    }
    else {
        step.factory()->printer()->print(result.message());
        return step.factory()->lazyInitStep(Factory::State::HOME);
    }
    return StepSwitcher::nextStep(step);
}

std::shared_ptr<Step> HomeStep::execute(Context &c) {
    std::stringstream ss{factory()->reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    command_ = command;

    factory()->controller()->setData(Controller::Data{arg});
    ActionResult result;
    std::shared_ptr<Controller> action;
    if (command_ == "edit" || command_ == "subtask" ||
        command_ == "delete" || command_ == "complete" ||
        command_ == "uncomplete" ||command_ == "label") {
        result = factory()->controller()->ValidateID(c);
    } else if (command_ == "show") {
        result = factory()->controller()->ValidateLabelOrID(c);
    } else if (command_ == "save" || command_ == "load" || command_.empty()) {
        result = factory()->controller()->ValidateAlpha(c);
    } else {
        result = factory()->controller()->ValidateNoArg(c);
    }

    return processResult(*this, result, "");
}

std::shared_ptr<Step> HelpStep::execute(Context &c) {
    FileReader fr("../src/model/help.txt");
    factory()->printer()->print(fr.read(""));
    return StepSwitcher::nextStep(*this);
}

std::shared_ptr<Step> AddStep::execute(Context &c) {
    factory()->printer()->print("[Add Task]\n");
    Machine wizard = factory()->createMachine(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    ActionResult result = factory()->controller()->AddTask(c);
    return processResult(*this, result, "Added Task");
}

bool ReadTaskDataStep::validateTitle(const std::string &title) const {
    if (title.empty()) {
        factory()->printer()->print("    Title cannot be empty!\n");
        return false;
    }
    return true;
}

std::optional<ProtoTask::Task::Priority> ReadTaskDataStep::stringToPriority(const std::string &priority) const {
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

std::optional<time_t> ReadTaskDataStep::stringToTime(const std::string &datestring) const {
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

void ReadTaskDataStep::readTitle(Context &c) const {
    std::string title;
    do {
        title = factory()->reader()->read("    Title > ");
    } while (!validateTitle(title));
    c.setTitle(title);
}

void ReadTaskDataStep::readPriority(Context &c) const {
    std::optional<ProtoTask::Task::Priority> priority{std::nullopt};
    while (!priority) {
        priority = stringToPriority(factory()->reader()->read("    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) > "));
    }
    c.setPriority(*priority);
}

void ReadTaskDataStep::readDueDate(Context &c) const {
    std::optional<time_t> due_date;
    while (!due_date) {
        due_date = stringToTime(factory()->reader()->read("    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)} > "));
    }
    c.setDueDate(*due_date);
}

std::shared_ptr<Step> ReadTaskDataStep::execute(Context &c) {
    readTitle(c);
    readPriority(c);
    readDueDate(c);
    return StepSwitcher::nextStep(*this);
}

std::shared_ptr<Step> EditStep::execute(Context &c) {
    factory()->printer()->print("[Edit Task]\n");
    Machine wizard = factory()->createMachine(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    ActionResult result = factory()->controller()->EditTask(c);
    return processResult(*this, result, "Edited Task");
}

std::shared_ptr<Step> SubtaskStep::execute(Context &c) {
    factory()->printer()->print("[Add Subtask]\n");
    Machine wizard = factory()->createMachine(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    ActionResult result = factory()->controller()->AddSubtask(c);
    return processResult(*this, result, "Added Subtask");
}

std::shared_ptr<Step> QuitStep::execute(Context &c) {
    return StepSwitcher::nextStep(*this);
}

std::shared_ptr<Step> ShowStep::execute(Context &c) {
    ActionResult result = factory()->controller()->ShowTasks(c);

    auto tasks = c.tasks();
    for (const auto &kv : tasks) {
        if (!kv.second.second.parent())
            ShowStep::recursivePrint(kv, c, "");
    }

    return StepSwitcher::nextStep(*this);
}

void ShowStep::recursivePrint(const std::pair<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> &kv,
                              const Context &c,
                              const std::string &prefix) {
    factory()->printer()->print(prefix + std::to_string(kv.first.value()) +
                     " – " + to_string(kv.second.first));

    factory()->printer()->print("\n");
    for (const auto &id : kv.second.second.children()) {
        auto ch = std::make_pair(id, c.tasks().at(id));
        recursivePrint(ch, c, prefix + "    ");
    }
}

std::shared_ptr<Step> CompleteStep::execute(Context &c) {
    ActionResult result = factory()->controller()->CompleteTask(c);
    return processResult(*this, result, "Completed Task");
}

std::shared_ptr<Step> UncompleteStep::execute(Context &c) {
    ActionResult result = factory()->controller()->UncompleteTask(c);
    return processResult(*this, result, "Uncompleted Task");
}

std::shared_ptr<Step> DeleteStep::execute(Context &c) {
    ActionResult result = factory()->controller()->DeleteTask(c);
    return processResult(*this, result, "Deleted Task");
}

std::shared_ptr<Step> ConfirmDeleteStep::execute(Context &c) {
    ActionResult result = factory()->controller()->ReadTaskWithChildren(c);

    if (result) {
        auto ch = c.tasks().at(*result.id).second.children();
        if (!ch.empty()) {
            while (true) {
                std::string reply = factory()->reader()->read("Task " + std::to_string(c.id()->value()) +
                                                              " has " + std::to_string(ch.size()) +
                                                              " subtask(s). Confirm to delete all. Y/[N] > ");
                if (reply == "Y" || reply == "y") {
                    break;
                } else if (reply.empty() || reply == "N" || reply == "n") {
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

    return StepSwitcher::nextStep(*this);
}

std::shared_ptr<Step> LabelStep::execute(Context &c) {
    std::string label = readLabel();
    factory()->controller()->setData(Controller::Data{label});
    ActionResult result = factory()->controller()->LabelTask(c);
    return processResult(*this, result, "Added label to Task");
}

std::string LabelStep::readLabel() const {
    std::string label = factory()->reader()->read("[Add Label]\n    >> ");
    while (label.empty())
        label = factory()->reader()->read("Label cannot be empty. Try again.\n    >> ");
    return label;
}

std::shared_ptr<Step> SaveStep::execute(Context &c) {
    ActionResult result = factory()->controller()->SaveTasks(c);
    return processResult(*this, result, "Saved to file successfully.\n");
}

std::shared_ptr<Step> LoadStep::execute(Context &c) {
    ActionResult result = factory()->controller()->LoadTasks(c);
    return processResult(*this, result, "Loaded from file successfully.\n");
}

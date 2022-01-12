//
// Created by Anton O. on 11/22/21.
//

#include "Step.h"
#include "Machine.h"
#include "StepSwitcher.h"

std::string HomeStep::command() const {
    return command_;
}

template <typename T>
std::shared_ptr<Step> processResult(const T &step,
                                    const std::shared_ptr<Factory> &factory,
                                    const ActionResult &result,
                                    const std::string &message) {
    if (result) {
        if (!message.empty()) {
            factory->printer()->print(message);
            if (result.id)
                factory->printer()->print(" (ID: " + std::to_string(result.id->value()) + ")\n");
        }
    }
    else {
        factory->printer()->print(result.message());
        return factory->lazyInitStep(Factory::State::HOME);
    }
    return StepSwitcher::nextStep(step, factory);
}

std::shared_ptr<Step> HomeStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    std::stringstream ss{f->reader()->read(" > ")};
    std::string command, arg;
    ss >> command >> arg;
    command_ = command;

    f->controller()->setData(Controller::Data{arg});
    ActionResult result;
    std::shared_ptr<Controller> action;
    if (command_ == "edit" || command_ == "subtask" ||
        command_ == "delete" || command_ == "complete" ||
        command_ == "uncomplete" ||command_ == "label") {
        result = f->controller()->ValidateID(c);
    } else if (command_ == "show") {
        result = f->controller()->ValidateAlphaOrID(c);
    } else if (command_ == "save" || command_ == "load") {
        result = f->controller()->ValidateAlpha(c);
    } else {
        result = f->controller()->ValidateNoArg(c);
    }

    return processResult(*this, f, result, "");
}

std::shared_ptr<Step> HelpStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    FileReader fr("../src/model/help.txt");
    f->printer()->print(fr.read(""));
    return StepSwitcher::nextStep(*this, f);
}

std::shared_ptr<Step> AddStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    f->printer()->print("[Add Task]\n");
    Machine wizard = f->createMachine(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    ActionResult result = f->controller()->AddTask(c);
    return processResult(*this, f, result, "Added Task");
}

bool ReadTaskDataStep::validateTitle(const std::string &title) const {
    return !title.empty();
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
        return std::nullopt;
    }
}

std::string ReadTaskDataStep::priorityToString(const ProtoTask::Task::Priority &priority) const {
    switch (priority) {
        case ProtoTask::Task_Priority_NONE:
            return "None";
        case ProtoTask::Task_Priority_LOW:
            return "Low";
        case ProtoTask::Task_Priority_MEDIUM:
            return "Medium";
        case ProtoTask::Task_Priority_HIGH:
            return "High";
        default:
            return "";
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
        return std::nullopt;
    }
}

std::string ReadTaskDataStep::timeToString(const time_t &date) const {
    std::string str{asctime(localtime(&date))};
    str.pop_back(); // remove trailing \n from asctime
    return str;
}

void ReadTaskDataStep::readTitle(Context &c, const std::shared_ptr<Factory> &f) const {
    std::string title;
    std::string prompt{"    Title"};
    if (!c.task().title().empty())
        prompt += (" [" + c.task().title() + "]");
    while (true) {
        title = f->reader()->read(prompt + " > ");
        title = title.empty() ? c.task().title() : title;
        if (!validateTitle(title))
            f->printer()->print("    Title cannot be empty!\n");
        else
            break;
    }
    c.setTitle(title);
}

void ReadTaskDataStep::readPriority(Context &c, const std::shared_ptr<Factory> &f) const {
    std::optional<ProtoTask::Task::Priority> priority{std::nullopt};
    std::string prompt = "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH)";
    if (c.task().priority() != ProtoTask::Task_Priority_NONE)
        prompt += (" [" + priorityToString(c.task().priority()) + "]");
    while (!priority) {
        std::string str = f->reader()->read(prompt + " > ");
        priority = str.empty() ? c.task().priority() : stringToPriority(str);
        if (!priority)
            f->printer()->print("    Wrong priority option. Try again.\n");
    }
    c.setPriority(*priority);
}

void ReadTaskDataStep::readDueDate(Context &c, const std::shared_ptr<Factory> &f) const {
    std::optional<time_t> due_date;
    std::string prompt = "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)}";
    if (c.task().due_date())
        prompt += (" [" + timeToString(c.task().due_date()) + "]");
    while (!due_date) {
        std::string str = f->reader()->read(prompt + " > ");
        if (str.empty() && c.task().due_date()) {
            due_date = std::make_optional(c.task().due_date());
        } else {
            due_date = stringToTime(str);
            if (!due_date)
                f->printer()->print("    Wrong date format. Try again.\n");
        }
    }
    c.setDueDate(*due_date);
}

std::shared_ptr<Step> ReadTaskDataStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    readTitle(c, f);
    readPriority(c, f);
    readDueDate(c, f);
    return StepSwitcher::nextStep(*this, f);
}

std::shared_ptr<Step> EditStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    f->printer()->print("[Edit Task]\n");
    ActionResult result_get_task = f->controller()->ReadTaskWithChildren(c);
    if (result_get_task)
        c.setTask(c.tasks()[0].data());
    Machine wizard = f->createMachine(Factory::State::READTASK, c);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    ActionResult result = f->controller()->EditTask(c);
    return processResult(*this, f, result, "Edited Task");
}

std::shared_ptr<Step> SubtaskStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    f->printer()->print("[Add Subtask]\n");
    Machine wizard = f->createMachine(Factory::State::READTASK);
    Context input_context = wizard.run();
    c.setTask(input_context.task());

    ActionResult result = f->controller()->AddSubtask(c);
    return processResult(*this, f, result, "Added Subtask");
}

std::shared_ptr<Step> QuitStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    return StepSwitcher::nextStep(*this, f);
}

std::shared_ptr<Step> ShowStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->ShowTasks(c);

    auto tasks = c.tasks();
    for (const auto &te : tasks) {
        if (!te.has_parent())
            ShowStep::recursivePrint(te, f, c, "");
    }

    return StepSwitcher::nextStep(*this, f);
}

void ShowStep::recursivePrint(const ProtoTask::TaskEntity &te,
                              const std::shared_ptr<Factory> &f,
                              const Context &c,
                              const std::string &prefix) {
    f->printer()->print(prefix + std::to_string(te.id().value()) +
                        " – " + to_string(te.data()));

    f->printer()->print("\n");
    for (const auto &ch : c.tasks()) {
        if (ch.parent() == te.id())
            recursivePrint(ch, f, c, prefix + "    ");
    }
}

std::shared_ptr<Step> CompleteStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->CompleteTask(c);
    return processResult(*this, f, result, "Completed Task");
}

std::shared_ptr<Step> UncompleteStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->UncompleteTask(c);
    return processResult(*this, f, result, "Uncompleted Task");
}

std::shared_ptr<Step> DeleteStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->DeleteTask(c);
    return processResult(*this, f, result, "Deleted Task");
}

std::shared_ptr<Step> ConfirmDeleteStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->ReadTaskWithChildren(c);

    if (result) {
        if (c.tasks().size() > 1) {
            while (true) {
                std::string reply = f->reader()->read("Task " + std::to_string(c.id()->value()) +
                                                              " has " + std::to_string(c.tasks().size()) +
                                                              " subtask(s). Confirm to delete all. Y/[N] > ");
                if (reply == "Y" || reply == "y") {
                    break;
                } else if (reply.empty() || reply == "N" || reply == "n") {
                    // disregard nextStep and go to HomeStep
                    return f->lazyInitStep(Factory::State::HOME);
                } else {
                    f->printer()->print("Wrong option. Type Y or N.\n");
                }
            }
        }
    } else {
        f->printer()->print(result.message());
        return f->lazyInitStep(Factory::State::HOME);
    }

    return StepSwitcher::nextStep(*this, f);
}

std::shared_ptr<Step> LabelStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    std::string label = readLabel(f);
    f->controller()->setData(Controller::Data{label});
    ActionResult result = f->controller()->LabelTask(c);
    return processResult(*this, f, result, "Added label to Task");
}

std::string LabelStep::readLabel(const std::shared_ptr<Factory> &f) const {
    std::string label = f->reader()->read("[Add Label]\n    >> ");
    while (label.empty())
        label = f->reader()->read("Label cannot be empty. Try again.\n    >> ");
    return label;
}

std::shared_ptr<Step> SaveStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->SaveTasks(c);
    return processResult(*this, f, result, "Saved to file successfully.\n");
}

std::shared_ptr<Step> LoadStep::execute(Context &c, const std::shared_ptr<Factory> &f) {
    ActionResult result = f->controller()->LoadTasks(c);
    return processResult(*this, f, result, "Loaded from file successfully.\n");
}

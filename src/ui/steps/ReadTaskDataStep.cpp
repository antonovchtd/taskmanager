//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "ReadTaskDataStep.h"
#include "ui/Context.h"

ReadTaskDataStep::ReadTaskDataStep(const std::shared_ptr<AbstractReader> &reader,
                                   const std::shared_ptr<AbstractPrinter> &printer) :
        IOStep(reader, printer) {
}

std::unique_ptr<Action> ReadTaskDataStep::genAction(Context &context) {
    readTitle(context);
    readPriority(context);
    readDueDate(context);
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> ReadTaskDataStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");
}

void ReadTaskDataStep::readTitle(Context &context) const {
    std::string title;
    std::string prompt{"    Title"};
    if (!context.task().title().empty())
        prompt += (" [" + context.task().title() + "]");
    while (true) {
        title = reader()->read(prompt + " > ");
        title = title.empty() ? context.task().title() : title;
        if (!validateTitle(title))
            printer()->print("    Title cannot be empty!\n");
        else
            break;
    }
    context.setTitle(title);
}

void ReadTaskDataStep::readPriority(Context &context) const {
    std::optional<Core::Task::Priority> priority{std::nullopt};
    std::string prompt = "    priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH)";
    if (context.task().priority() != Core::Task_Priority_NONE)
        prompt += (" [" + priorityToString(context.task().priority()) + "]");
    while (!priority) {
        std::string str = reader()->read(prompt + " > ");
        priority = str.empty() ? context.task().priority() : stringToPriority(str);
        if (!priority)
            printer()->print("    Wrong priority option. Try again.\n");
    }
    context.setPriority(*priority);
}

void ReadTaskDataStep::readDueDate(Context &context) const {
    std::optional<time_t> due_date;
    std::string prompt = "    Due {Format: dd[/.]mm[/.](/(yy)yy) (hh:mm)}";
    if (context.task().due_date())
        prompt += (" [" + timeToString(context.task().due_date()) + "]");
    while (!due_date) {
        std::string str = reader()->read(prompt + " > ");
        if (str.empty() && context.task().due_date()) {
            due_date = std::make_optional(context.task().due_date());
        } else {
            due_date = stringToTime(str);
            if (!due_date)
                printer()->print("    Wrong date format. Try again.\n");
        }
    }
    context.setDueDate(*due_date);
}

std::optional<time_t> ReadTaskDataStep::ReadTaskDataStep::stringToTime(const std::string &datestring) const {
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

bool ReadTaskDataStep::validateTitle(const std::string &title) const {
    return !title.empty();
}

std::optional<Core::Task::Priority> ReadTaskDataStep::stringToPriority(const std::string &priority) const {
    int pint;
    try {
        pint = priority.empty() ? 0 : std::stoi(priority);
    } catch (const std::invalid_argument &) {
        pint = -1;
    }
    if (pint >= 0 && pint <= 3)
        return static_cast<Core::Task::Priority>(pint);
    else {
        return std::nullopt;
    }
}

std::string ReadTaskDataStep::priorityToString(const Core::Task::Priority &priority) const {
    switch (priority) {
        case Core::Task_Priority_NONE:
            return "None";
        case Core::Task_Priority_LOW:
            return "Low";
        case Core::Task_Priority_MEDIUM:
            return "Medium";
        case Core::Task_Priority_HIGH:
            return "High";
        default:
            return "";
    }
}

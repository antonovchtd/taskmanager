//
// Created by Anton Ovcharenko on 08.11.2021.
//
#include <iostream>
#include <regex>

#include "Task.h"

Task::Task(std::string title, Task::Priority p, time_t due_date, bool complete_flag) :
    title_{std::move(title)},
    priority_{p},
    due_date_{due_date},
    isComplete_(complete_flag)
{
    if (title_.empty())
        throw std::runtime_error("Empty title string.");
};

Task::Task(std::string title, Task::Priority p, const std::string &due_date, bool complete_flag) :
        Task(std::move(title), p, 0, complete_flag)
{
    std::smatch matches;
    if (std::regex_search(due_date, matches, std::regex(R"(in (\d+:)?(\d+):(\d+))"))){
        due_date_ = time(nullptr) + std::atoi(matches.str(1).c_str())*24*3600
                + std::atoi(matches.str(2).c_str())*3600
                + std::atoi(matches.str(3).c_str())*60;
    }
    else if (std::regex_search(due_date, matches, std::regex(R"((\d+)/(\d+)(/(\d+))?( (\d+):(\d+))?)"))){
        time_t rawtime;
        time(&rawtime);
        struct tm * timeinfo = localtime(&rawtime);
        timeinfo->tm_mday = std::atoi(matches.str(1).c_str());
        timeinfo->tm_mon = std::atoi(matches.str(2).c_str())-1;
        if (!matches.str(3).empty()){
            int year = std::atoi(matches.str(4).c_str());
            if (year > 1900)
                year -= 1900;
            else
                year += 100;
            timeinfo->tm_year = year;
        }
        if (!matches.str(5).empty()) {
            timeinfo->tm_hour = std::atoi(matches.str(6).c_str());
            timeinfo->tm_min = std::atoi(matches.str(7).c_str());
        }
        else
        {
            timeinfo->tm_hour = 0;
            timeinfo->tm_min = 0;
        }
        timeinfo->tm_sec = 0;
        due_date_ = mktime(timeinfo);
    }

};

Task Task::Create(const Task::Data &d) {
    return Task::Create(d.title, d.priority, d.due_date, false);
}

Task Task::Create(const std::string &title, Task::Priority p, time_t due_date, bool complete_flag) {
    return {title, p, due_date, complete_flag};
}

Task Task::Create(const std::string &title, Task::Priority p, const std::string &due_date, bool complete_flag) {
    return {title, p, due_date, complete_flag};
}

bool Task::operator==(const Task& rhs) const{
    return title_ == rhs.title()
        && priority_ == rhs.priority()
        && due_date_ == rhs.due_date();
}

std::ostream & operator<<(std::ostream &os, const Task& t) {
    const std::string priorityName[] = { "None", "Low", "Medium", "High"};
    os << t.title() << ", Priority: " <<
       priorityName[static_cast<int>(t.priority())];
    if (t.due_date() < time(nullptr))
        os << " [overdue] ";
    else {
        time_t dd = t.due_date();
        std::string str_time = std::string(asctime(localtime(&dd)));
        str_time.pop_back();
        os << ", Due: " << str_time;
    }
    if (t.isComplete()) {
        os << " [completed] ";
    }
    os << std::endl;
    return os;
}
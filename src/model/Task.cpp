//
// Created by Anton Ovcharenko on 08.11.2021.
//
#include <regex>
#include <iostream>

#include "Task.h"

Task::Task(std::string title, Task::Priority p, time_t due_date) :
title_{std::move(title)},
priority_{p},
due_date_{time(nullptr) + due_date}
{
    if (title_.empty())
        throw std::runtime_error("Empty title string.");
};

Task::Task(std::string title, Task::Priority p, const std::string &due_date) :
        Task(std::move(title),p,0)
{
    std::smatch matches;
    if (std::regex_search(due_date, matches, std::regex(R"(in (\d+:)?(\d+):(\d)+)"))){
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

Task Task::Create(const std::string &title, Task::Priority p, time_t due_date) {
    return {title, p, due_date};
}

Task Task::Create(const std::string &title, Task::Priority p) {
    return {title, p, 0};
}

Task Task::Create(const std::string &title, time_t due_date) {
    return {title, Task::Priority::NONE, due_date};
}

Task Task::Create(const std::string &title) {
    return {title, Task::Priority::NONE, 0};
}

Task Task::Create(const std::string &title, Task::Priority p, const std::string &due_date) {
    return {title, p, due_date};
}

Task Task::Create(const std::string &title, const std::string &due_date) {
    return {title, Task::Priority::NONE, due_date};
}

void Task::setComplete(bool status) {
    isComplete_ = status;
}

bool Task::operator==(const Task& rhs) const{
    return title_ == rhs.title()
        && priority_ == rhs.priority()
        && due_date_ == rhs.due_date();
}

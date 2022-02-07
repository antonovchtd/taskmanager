//
// Created by Anton Ovcharenko on 04.02.2022.
//

#ifndef TASKMANAGER_SRC_LOGGING_LOGGER_H_
#define TASKMANAGER_SRC_LOGGING_LOGGER_H_

#include "LoggerUtils.h"

BOOST_LOG_ATTRIBUTE_KEYWORD(process_id, "ProcessID", attrs::current_process_id::value_type)

class Logger {
public:
    void init();

public:
    Logger& SetFilename(const std::string &filename);
    Logger& PrintToConsole(bool print_to_console);
    Logger& SetFilterLevel(const logging::trivial::severity_level &level);

private:
    std::optional<std::string> filename_;
    std::optional<bool> print_to_console_;
    std::optional<logging::trivial::severity_level> level_;
};


#endif //TASKMANAGER_SRC_LOGGING_LOGGER_H_

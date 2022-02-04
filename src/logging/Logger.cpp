//
// Created by Anton Ovcharenko on 04.02.2022.
//

#include "Logger.h"

void Logger::init() {
    logging::core::get()->add_global_attribute("File", attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_global_attribute("Function", attrs::mutable_constant<std::string>(""));

    auto MyFormat = expr::stream
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
            << ": [" << boost::phoenix::bind(&get_native_process_id<logging::value_ref<attrs::current_process_id::value_type,tag::process_id>>,
    process_id.or_none())
    << "] [" << logging::trivial::severity << "] "
             << expr::if_(logging::trivial::severity <= logging::trivial::debug)
             [
                     expr::stream
                             << expr::attr<std::string>("File") << "/"
                             << expr::attr<std::string>("Function") << ": "
             ]
             << expr::smessage;

    logging::add_file_log
            (
                    keywords::file_name = filename_.value_or("taskmanager.log"),
                    keywords::rotation_size = 5 * 1024 * 1024,
                    keywords::format = MyFormat,
                    keywords::open_mode = std::ios_base::out | std::ios_base::app,
                    keywords::auto_flush = true
            );

    if (print_to_console_.value_or(false))
        logging::add_console_log(std::cout, keywords::format = MyFormat);

    logging::core::get()->set_filter
            (
                    logging::trivial::severity >= level_.value_or(logging::trivial::info)
            );

    logging::add_common_attributes();
}

Logger& Logger::SetFilename(const std::string &filename) {
    filename_ = filename;
    return *this;
}

Logger& Logger::PrintToConsole(bool print_to_console) {
    print_to_console_ = print_to_console;
    return *this;
}

Logger& Logger::SetFilterLevel(const logging::trivial::severity_level &level) {
    level_ = level;
    return *this;
}

//
// Created by Anton Ovcharenko on 04.02.2022.
//

#ifndef TASKMANAGER_SRC_LOGGER_LOGGERUTILS_H_
#define TASKMANAGER_SRC_LOGGER_LOGGERUTILS_H_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/current_process_id.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/expressions/formatters/if.hpp>
#include <boost/phoenix.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

#define LOG_STREAM(sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      (logging::trivial::logger::get()), \
         (set_get_attrib("Function", extract_class_function_name(__PRETTY_FUNCTION__))) \
         (set_get_attrib("File", extract_filename(__FILE__))) \
         (keywords::severity = (logging::trivial::sev)) \
   )

template<typename ValueType>
ValueType set_get_attrib(const char* name, ValueType value) {
    auto attr = logging::attribute_cast<attrs::mutable_constant<ValueType>>(logging::core::get()->get_global_attributes()[name]);
    attr.set(value);
    return attr.get();
}

std::string extract_filename(const std::string &path);
std::string extract_class_function_name(std::string path);

template<typename ValueType>
attrs::current_process_id::value_type::native_type get_native_process_id(
        ValueType const& pid){
    if (pid)
        return pid->native_id();
    return 0;
}

#endif //TASKMANAGER_SRC_LOGGER_LOGGERUTILS_H_

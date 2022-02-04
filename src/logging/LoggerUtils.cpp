//
// Created by Anton Ovcharenko on 04.02.2022.
//

#include "LoggerUtils.h"

std::string extract_filename(const std::string &path) {
    return path.substr(path.find_last_of("/\\")+1);
}

std::string extract_class_function_name(std::string path) {
    path.erase(path.find('('));
    return path.substr(path.find_last_of(' ')+1);
}

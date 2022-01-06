//
// Created by Anton O. on 12/6/21.
//

#include "ConsoleReader.h"

static inline void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

std::string ConsoleReader::read(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    getline(std::cin, input);
    trim(input);
    return input;
}
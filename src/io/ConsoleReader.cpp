//
// Created by Anton O. on 12/6/21.
//

#include "ConsoleReader.h"

std::string ConsoleReader::read(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    getline(std::cin, input);
    return input;
}
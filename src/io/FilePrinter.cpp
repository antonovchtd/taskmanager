//
// Created by Anton O. on 12/6/21.
//

#include "FilePrinter.h"

FilePrinter::FilePrinter(const std::string &filename) : filename_(filename) {
}

void FilePrinter::print(const std::string &message) {
    std::ofstream file(filename_);
    if (file.is_open()) {
        file << message;
        file.close();
    } else {
        throw std::runtime_error("FilePrinter::print failed to open file " + filename_);
    }
}
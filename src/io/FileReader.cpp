//
// Created by Anton O. on 12/6/21.
//

#include "FileReader.h"

FileReader::FileReader(std::string filename) : filename_(std::move(filename)) {
}

std::string FileReader::read(const std::string &message) {
    std::ifstream file(filename_);
    std::ostringstream os;
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            os << line << "\n";
        file.close();
    } else {
        os << "FileReader::read failed to open file " + filename_ << std::endl;
    }
    return os.str();
}
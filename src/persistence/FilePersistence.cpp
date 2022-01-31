//
// Created by Anton Ovcharenko on 15.12.2021.
//

#include <google/protobuf/util/delimited_message_util.h>

#include "FilePersistence.h"
#include "model/TaskManager.h"

FilePersistence::FilePersistence(const std::string &filename) : filename_{filename} {
}

bool FilePersistence::save(const std::vector<Core::TaskEntity> &vec) {
    std::ofstream file(filename_, std::ios::trunc | std::ios::binary);
    if (file.is_open()) {
        for (const auto &te : vec) {
            google::protobuf::util::SerializeDelimitedToOstream(te, &file);
        }
        file.close();
        return true;
    }
    else
        return false;
}

bool FilePersistence::load(std::vector<Core::TaskEntity> &vec) {
    std::ifstream file(filename_, std::ios::binary);
    google::protobuf::io::IstreamInputStream iis{&file};
    if (file.is_open()) {
        bool clean_eof = false;
        while (true) {
            Core::TaskEntity te;
            google::protobuf::util::ParseDelimitedFromZeroCopyStream(&te, &iis, &clean_eof);
            if (!clean_eof)
                vec.push_back(te);
            else
                break;
        }
        file.close();
        return true;
    }
    else
        return false;
}
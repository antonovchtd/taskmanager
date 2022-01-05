//
// Created by Anton Ovcharenko on 15.12.2021.
//

#include <google/protobuf/util/delimited_message_util.h>

#include "Persister.h"
#include "model/TaskManager.h"

std::string Persister::defaultFilename() const {
    return default_filename_;
}

bool Persister::save(const std::string &filename, const std::shared_ptr<TaskManagerInterface> &model) {
    std::ofstream file(filename, std::ios::trunc | std::ios::binary);
    if (file.is_open()) {
        std::vector<ProtoTask::TaskEntity> vec = model->Export();
        for (const auto &te : vec) {
            google::protobuf::util::SerializeDelimitedToOstream(te, &file);
        }
        file.close();
        return true;
    }
    else
        return false;
}

bool Persister::load(const std::string &filename, const std::shared_ptr<TaskManagerInterface> &model) {
    std::ifstream file(filename, std::ios::binary);
    google::protobuf::io::IstreamInputStream iis{&file};
    if (file.is_open()) {
        ProtoTask::TaskEntity te;
        std::vector<ProtoTask::TaskEntity> vec;
        bool clean_eof = false;
        while (!clean_eof) {
            google::protobuf::util::ParseDelimitedFromZeroCopyStream(&te, &iis, &clean_eof);
            vec.push_back(te);
        }
        file.close();
        model->Replace(vec);
        return true;
    }
    else
        return false;
}
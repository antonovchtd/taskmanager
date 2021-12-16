//
// Created by Anton Ovcharenko on 15.12.2021.
//

#include <google/protobuf/util/delimited_message_util.h>

#include "Persistor.h"
#include "../model/TaskManager.h"

void Persistor::save(const TaskManager &tm, std::ostream &os) {
    google::protobuf::util::SerializeDelimitedToOstream(tm.gen()->state(), &os);
    for (const auto &kv : tm.getTasks()) {
        ProtoTask::TaskEntity te;
        te.set_allocated_id(new ProtoTask::TaskID(kv.first));
        te.set_allocated_data(new ProtoTask::Task(kv.second.first));
        if (kv.second.second.parent()) {
            auto parent = new ProtoTask::TaskID(*kv.second.second.parent());
            te.set_allocated_parent(parent);
        }
        google::protobuf::util::SerializeDelimitedToOstream(te,&os);
    }
}

TaskManager Persistor::load(std::istream &is) {
    ProtoTask::IDGeneratorState state;
    google::protobuf::io::IstreamInputStream iis{&is};
    bool clean_eof;
    google::protobuf::util::ParseDelimitedFromZeroCopyStream(&state, &iis, &clean_eof);

    std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> tasks;
    ProtoTask::TaskEntity te;
    while (!clean_eof) {
        google::protobuf::util::ParseDelimitedFromZeroCopyStream(&te, &iis, &clean_eof);
        Node n;
        if (te.has_parent())
            n.SetParent(te.parent());
        tasks.insert(std::make_pair(te.id(),
                                    std::make_pair(te.data(), n)));
    }

    for (const auto &kv : tasks)
        if (kv.second.second.parent())
            tasks[*kv.second.second.parent()].second.AddChild(kv.first);

    return {std::make_shared<IDGenerator>(state), tasks};
}
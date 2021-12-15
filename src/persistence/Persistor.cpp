//
// Created by Anton Ovcharenko on 15.12.2021.
//

#include "Persistor.h"
#include "../model/TaskManager.h"

void Persistor::save(const TaskManager &tm, std::ostream &os) {
    tm.gen()->state().SerializeToOstream(&os);
    for (const auto &kv : tm.getTasks()) {
        ProtoTask::TaskEntity te;
        auto id = new ProtoTask::TaskID(kv.first);
        te.set_allocated_id(id);
        auto task = new ProtoTask::Task(kv.second.first);
        te.set_allocated_data(task);
        if (kv.second.second.parent()) {
            auto parent = new ProtoTask::TaskID(*kv.second.second.parent());
            te.set_allocated_parent(parent);
        }
        te.SerializeToOstream(&os);
    }
}

TaskManager Persistor::load(std::istream &is) {
    ProtoTask::IDGeneratorState state;
//    state.ParseFromIstream(&is);

    std::map<ProtoTask::TaskID, std::pair<ProtoTask::Task, Node>> tasks;
    ProtoTask::TaskEntity te;
    Node n;
    while (!is.eof()) {
        te.ParseFromIstream(&is);
        if (te.has_parent())
            n.SetParent(te.parent());
        tasks.insert(std::make_pair(te.id(),
                                    std::make_pair(te.data(), n)));
    }

    return {std::make_shared<IDGenerator>(state), tasks};
}
//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_LOADFROMFILEACTION_H
#define TASKMANAGER_LOADFROMFILEACTION_H

#include "Action.h"
#include "persistence/Persister.h"

class LoadFromFileAction : public Action {
public:
    LoadFromFileAction(const std::string &filename);

public:
    ActionResult execute(Context &, const std::shared_ptr<ModelInterface> &) override;

private:
    std::string filename_;
    std::unique_ptr<Persister> persister_;
};


#endif //TASKMANAGER_LOADFROMFILEACTION_H

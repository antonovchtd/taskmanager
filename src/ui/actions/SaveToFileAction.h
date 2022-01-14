//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_SAVETOFILEACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_SAVETOFILEACTION_H_

#include "Action.h"
#include "persistence/Persister.h"

class SaveToFileAction : public Action {
public:
    SaveToFileAction(const std::string &filename);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::string filename_;
    std::unique_ptr<Persister> persister_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_SAVETOFILEACTION_H_

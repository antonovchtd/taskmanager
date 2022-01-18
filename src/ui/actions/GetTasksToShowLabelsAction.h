//
// Created by Anton Ovcharenko on 18.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_GETTASKSTOSHOWLABELSACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_GETTASKSTOSHOWLABELSACTION_H_

#include "Action.h"

class GetTasksToShowLabelsAction : public Action {
public:
    explicit GetTasksToShowLabelsAction(const std::string &arg);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::string arg_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_GETTASKSTOSHOWLABELSACTION_H_

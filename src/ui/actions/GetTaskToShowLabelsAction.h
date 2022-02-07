//
// Created by Anton Ovcharenko on 18.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_GETTASKTOSHOWLABELSACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_GETTASKTOSHOWLABELSACTION_H_

#include "Action.h"

class GetTaskToShowLabelsAction : public Action {
public:
    explicit GetTaskToShowLabelsAction(const std::optional<Core::TaskID> &id);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::optional<Core::TaskID> id_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_GETTASKTOSHOWLABELSACTION_H_

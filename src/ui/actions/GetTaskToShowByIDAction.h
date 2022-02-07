//
// Created by Anton Ovcharenko on 31.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_GETTASKTOSHOWBYIDACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_GETTASKTOSHOWBYIDACTION_H_

#include "Action.h"

class GetTaskToShowByIDAction : public Action {
public:
    explicit GetTaskToShowByIDAction(const Core::TaskID &id);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_GETTASKTOSHOWBYIDACTION_H_

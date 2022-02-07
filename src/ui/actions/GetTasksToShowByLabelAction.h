//
// Created by Anton Ovcharenko on 31.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_GETTASKSTOSHOWBYLABELACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_GETTASKSTOSHOWBYLABELACTION_H_

#include "Action.h"

class GetTasksToShowByLabelAction : public Action {
public:
    explicit GetTasksToShowByLabelAction(const std::string &label);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::string label_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_GETTASKSTOSHOWBYLABELACTION_H_

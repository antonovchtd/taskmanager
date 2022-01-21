//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_SHOWTASKSACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_SHOWTASKSACTION_H_

#include "Action.h"

class GetTasksToShowAction : public Action {
public:
    explicit GetTasksToShowAction(const std::optional<Core::TaskID> &id);
    explicit GetTasksToShowAction(const std::string &arg);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    std::optional<Core::TaskID> id_;
    std::string arg_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_SHOWTASKSACTION_H_

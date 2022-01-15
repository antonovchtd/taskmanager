//
// Created by Anton Ovcharenko on 14.01.2022.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_EDITTASKACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_EDITTASKACTION_H_

#include "Action.h"

class EditTaskAction : public Action {
public:
    EditTaskAction(const Core::TaskID &, const Core::Task &);
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
    Core::Task task_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_EDITTASKACTION_H_

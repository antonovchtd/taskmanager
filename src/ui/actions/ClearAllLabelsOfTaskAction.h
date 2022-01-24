//
// Created by Anton O. on 1/17/22.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_CLEARALLLABELSOFTASKSACTION_H_
#define TASKMANAGER_SRC_UI_ACTIONS_CLEARALLLABELSOFTASKSACTION_H_

#include "Action.h"

class ClearAllLabelsOfTaskAction : public Action {
public:
    explicit ClearAllLabelsOfTaskAction(const Core::TaskID &);

public:
    ActionResult execute(const std::shared_ptr<ModelInterface> &) override;

private:
    Core::TaskID id_;
};


#endif //TASKMANAGER_SRC_UI_ACTIONS_CLEARALLLABELSOFTASKSACTION_H_
//
// Created by Anton O. on 12/18/21.
//

#ifndef TASKMANAGER_SRC_UI_ACTIONS_ACTIONRESULT_H_
#define TASKMANAGER_SRC_UI_ACTIONS_ACTIONRESULT_H_

#include <optional>
#include "Core.pb.h"

struct ActionResult {
public:
    ActionResult(const Core::ModelInquiryResult &); // not explicit by design
    explicit ActionResult(const Core::TaskEntity &);
    explicit ActionResult(const std::vector<Core::TaskEntity> &);

public:
    explicit operator bool() const;

public:
    union {
        Core::ModelInquiryResult model_result;
        Core::TaskEntity entity;
        std::vector<Core::TaskEntity> tasks;
    };
    enum {kResult, kEntity, kVector} type_id;

public:
    ~ActionResult();
};

#endif //TASKMANAGER_SRC_UI_ACTIONS_ACTIONRESULT_H_

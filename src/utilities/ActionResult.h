//
// Created by Anton O. on 12/18/21.
//

#ifndef TASKMANAGER_SRC_MODEL_ACTIONRESULT_H_
#define TASKMANAGER_SRC_MODEL_ACTIONRESULT_H_

#include <optional>
#include "Task.pb.h"

struct ActionResult {
    enum class Status {
        SUCCESS,
        ID_NOT_FOUND,
        PARENT_ID_NOT_FOUND,
        HAS_CHILDREN,
        DUPLICATE_ID,
        FILE_NOT_FOUND,
        FAILED_TO_OPEN_FILE,
        TAKES_NO_ARG,
        TAKES_ARG,
        TAKES_ID,
        TAKES_ALPHA_NOT_ID,
        TAKES_ALPHA
    };

public:
    ActionResult(Status, const std::optional<Core::TaskID> &);
    ActionResult(Status, const Core::TaskEntity &);
    ActionResult(Status, const std::vector<Core::TaskEntity> &);

public:
    explicit operator bool() const;
    std::string message() const;

public:
    Status status;
    union {
        std::optional<Core::TaskID> id;
        Core::TaskEntity entity;
        std::vector<Core::TaskEntity> tasks;
    };
    enum {kID, kEntity, kVector} type_id;

public:
    ~ActionResult();
};

#endif //TASKMANAGER_SRC_MODEL_ACTIONRESULT_H_

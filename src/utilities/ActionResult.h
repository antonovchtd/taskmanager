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

    explicit operator bool() const;
    std::string message() const;

    Status status;
    std::optional<ProtoTask::TaskID> id;
};

#endif //TASKMANAGER_SRC_MODEL_ACTIONRESULT_H_

//
// Created by Anton Ovcharenko on 25.01.2022.
//

#include "TaskManagerService.h"

Transfer::ActionResult ActionResultTransformer(const ActionResult &old_result) {
    Transfer::ActionResult new_result;
    switch (old_result.status) {
        case ActionResult::Status::SUCCESS:
            new_result.set_status(Transfer::ActionResult_Status_SUCCESS);
            break;
        case ActionResult::Status::ID_NOT_FOUND:
            new_result.set_status(Transfer::ActionResult_Status_ID_NOT_FOUND);
            break;
        // TODO
    }
    switch (old_result.type_id) {
        case ActionResult::kID:
            new_result.mutable_id()->CopyFrom(*old_result.id);
            break;
        case ActionResult::kEntity:
            new_result.mutable_tasks()->Add(old_result.tasks.begin(),
                                            old_result.tasks.end());
            break;
    }
    return new_result;
}

grpc::Status TaskManagerService::Add(grpc::ServerContext* context, const Core::Task* task,
                                     Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->Add(*task));
    return grpc::Status::OK;
}
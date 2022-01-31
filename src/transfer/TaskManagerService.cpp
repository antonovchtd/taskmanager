//
// Created by Anton Ovcharenko on 25.01.2022.
//

#include "TaskManagerService.h"

TaskManagerService::TaskManagerService(const std::shared_ptr<ModelInterface> &model) :
                    model_{model} {
}

Transfer::ManyTaskEntities ManyTaskEntitiesTransformer(const std::vector<Core::TaskEntity> &vec) {
    Transfer::ManyTaskEntities mte;
    mte.mutable_tasks()->Add(vec.begin(), vec.end());
    return mte;
}

grpc::Status TaskManagerService::getTasks(grpc::ServerContext* context,
                                          const Transfer::EmptyMessage* msg,
                                          Transfer::ManyTaskEntities* result) {
    *result = ManyTaskEntitiesTransformer(model_->getTasks());
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::getTasksByLabel(grpc::ServerContext* context,
                                          const Core::Label* label,
                                          Transfer::ManyTaskEntities* result) {
    *result = ManyTaskEntitiesTransformer(model_->getTasks(label->label()));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::getTaskWithSubtasks(grpc::ServerContext* context,
                                                     const Core::TaskID* id,
                                                     Transfer::ManyTaskEntities* result) {
    *result = ManyTaskEntitiesTransformer(model_->getTaskWithSubtasks(*id));
    return grpc::Status::OK;
}

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

grpc::Status TaskManagerService::AddSubtask(grpc::ServerContext* context,
                                            const Core::TaskEntity* te,
                                            Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->AddSubtask(te->data(), te->id()));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::Edit(grpc::ServerContext* context,
                                      const Core::TaskEntity* te,
                                      Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->Edit(te->id(), te->data()));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::Complete(grpc::ServerContext* context,
                                          const Core::TaskID* id,
                                          Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->Complete(*id));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::Uncomplete(grpc::ServerContext* context,
                                            const Core::TaskID* id,
                                            Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->Uncomplete(*id));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::Delete(grpc::ServerContext* context,
                                        const Core::TaskID* id,
                                        Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->Delete(*id, true));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::IsPresent(grpc::ServerContext* context,
                                           const Core::TaskID* id,
                                           Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->IsPresent(*id));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::AddLabel(grpc::ServerContext *context,
                                          const Transfer::IDWithLabel *msg,
                                          Transfer::ActionResult *result) {
    *result = ActionResultTransformer(model_->AddLabel(msg->id(), msg->label().label()));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::ClearLabel(grpc::ServerContext *context,
                                            const Transfer::IDWithLabel *msg,
                                            Transfer::ActionResult *result) {
    *result = ActionResultTransformer(model_->RemoveLabel(msg->id(), msg->label().label()));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::ClearLabels(grpc::ServerContext* context,
                                             const Core::TaskID* id,
                                             Transfer::ActionResult* result) {
    *result = ActionResultTransformer(model_->ClearLabels(*id));
    return grpc::Status::OK;
}

grpc::Status TaskManagerService::Replace(grpc::ServerContext* context,
                                         const Transfer::ManyTaskEntities* mte,
                                         Transfer::EmptyMessage* result) {
    std::vector<Core::TaskEntity> vec(mte->tasks().begin(), mte->tasks().end());
    model_->Replace(vec);
    return grpc::Status::OK;
}

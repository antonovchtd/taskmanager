//
// Created by Anton O. on 1/5/22.
//

#ifndef TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_
#define TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_

#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <utility>
#include <fstream>

#include "Core.pb.h"
class IDGenerator;

/**
 * \brief API for Task Manager's core model.
 * \details With the use of Protobuf-generated classes.
 * \author Anton Ovcharenko
 */

class ModelInterface {
public:
    /**
     * Returns all tasks in the model.
     * @return std::vector of Core::TaskEntity objects
     */
    virtual std::vector<Core::TaskEntity> getTasks() = 0;
    /**
     * Returns all tasks by label, regardless of their position in the hierarchy.
     * @param label Label to filter by, as a Core::Label object.
     * @return std::vector of Core::TaskEntity objects
     */
    virtual std::vector<Core::TaskEntity> getTasks(const Core::Label &label) = 0;
    /**
     * Returns task by its ID including all of its subtasks.
     * @param id ID of the task in question as Core::TaskID object.
     * @return std::vector of Core::TaskEntity objects
     */
    virtual std::vector<Core::TaskEntity> getTaskWithSubtasks(const Core::TaskID &id) = 0;

public:
    /**
     * Adds new task, returns its ID, if successful, or an error enum otherwise, as
     * defined in Core::ModelRequestResult.
     * @param task Core::Task object as a DTO of the Task to add.
     * @return Core::ModelRequestResult with the new Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult Add(const Core::Task &) = 0;
    /**
     * Adds new task, as a subtask of already existing Task.
     * Returns new task's ID, if successful, or an error enum otherwise, as
     * defined in Core::ModelRequestResult.
     * @param task Core::Task object as a DTO of the Task to add.
     * @param id Core::TaskID object of the parent Task.
     * @return Core::ModelRequestResult with the new Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult AddSubtask(const Core::Task &, const Core::TaskID &) = 0;
    /**
     * Substitutes Task with the given ID with the new provided object.
     * Returns its ID, if successful, or an error enum otherwise, as
     * defined in Core::ModelRequestResult.
     * @param id Core::TaskID object that matches the one of the Task to edit.
     * @param task Core::Task object as a DTO of the new Task.
     * @return Core::ModelRequestResult with the edited Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult Edit(const Core::TaskID &id, const Core::Task &t) = 0;
    /**
     * Marks a given task with all its subtasks as completed.
     * @param id Core::TaskID object of the Task to complete.
     * @return Core::ModelRequestResult with the completed Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult Complete(const Core::TaskID &) = 0;
    /**
     * Set's a given Task's (and all of its subtasks) completed flag as false.
     * @param id Core::TaskID object of the Task to mark as uncompleted.
     * @return Core::ModelRequestResult with the uncompleted Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult Uncomplete(const Core::TaskID &) = 0;
    /**
     * Deletes the specified task from the model with all of its children (subtasks).
     * @param id Core::TaskID object of the Task to be deleted.
     * @param deleteChildren if false and has children, the action is aborted
     * @return Core::ModelRequestResult with the deleted Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult Delete(const Core::TaskID &, bool deleteChildren) = 0;
    /**
     * Looks up the task by its ID. Returns ID if present, or
     * Core::ModelRequestResult_Status_ID_NOT_FOUND otherwise.
     * @param id Core::TaskID object of the Task to look for.
     * @return Core::ModelRequestResult with the Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult CheckTask(const Core::TaskID &) = 0;
    /**
     * Adds label to the list of labels of Task with the given ID.
     * @param id Core::TaskID object of the Task to add label to.
     * @param label Core::Label object to add to repeated label vector.
     * @return Core::ModelRequestResult with the Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult AddLabel(const Core::TaskID &, const Core::Label &label) = 0;
    /**
     * Removes the given label from the list of labels of the Task with the given ID.
     * @param id Core::TaskID object of the Task to remove the label from.
     * @param label Core::Label object to be removed from the repeated label vector.
     * @return Core::ModelRequestResult with the Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult RemoveLabel(const Core::TaskID &, const Core::Label &label) = 0;
    /**
     * Removes all labels from the list of labels of the Task with the given ID.
     * @param id Core::TaskID object of the Task to remove the labels from.
     * @return Core::ModelRequestResult with the Task's ID (if success) or error status (if fail)
     */
    virtual Core::ModelRequestResult RemoveAllLabels(const Core::TaskID &) = 0;

public:
    /**
     * Replaces all tasks in the model with the ones given by the vector of TaskEntitites.
     * @param tasks tasks to add to the model, replacing any that were there before.
     */
    virtual void Replace(const std::vector<Core::TaskEntity> &tasks) = 0;

public:
    virtual ~ModelInterface() = default;

};

#endif //TASKMANAGER_SRC_MODEL_TASKMANAGERINTERFACE_H_

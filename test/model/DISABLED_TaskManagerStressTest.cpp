//
// Created by Anton O. on 2/17/22.
//

#include <random>
#include <thread>
#include <mutex>

#include "model/TaskManager.h"
#include "utilities/TaskEntityUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::Invoke;

class DISABLED_TaskManagerStressTest : public ::testing::Test
{

};

enum class Command{
    GETALL = 0,
    GET_BY_LABEL,
    GET_BY_ID,
    ADD,
    EDIT,
    SUBTASK,
    COMPLETE,
    UNCOMPLETE,
    DELETE,
    IS_PRESENT,
    LABEL,
    UNLABEL,
    UNLABEL_ALL
};

class MockIDGenerator : public IDGenerator {
public:
    explicit MockIDGenerator(int last) {last_ = last;};
    MockIDGenerator() {last_ = 1;};
    MOCK_METHOD(Core::TaskID, genID, (), (override));
};

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

Core::TaskID getRandomID(const TaskManager &tm) {
    auto tasks = tm.getTasks();
    auto sz = tasks.size();
    if (sz > 0) {
        std::uniform_int_distribution<unsigned long> uni(0, sz-1);
        auto random_position = uni(rng);
        return tasks[random_position].id();
    } else {
        Core::TaskID id;
        id.set_value(0);
        return id;
    }
}

Core::Label getRandomLabel() {
    Core::Label label;
    std::uniform_int_distribution<int> uni(0, 50); // guaranteed unbiased
    auto random_integer = uni(rng);
    label.set_str(std::to_string(random_integer));
    return label;
}

std::vector<Core::TaskEntity> runCommand(const Command &command, TaskManager &tm) {
    std::vector<Core::TaskEntity> res;


    Core::TaskID id;
    if (command == Command::IS_PRESENT)
            id.set_value(rand());

    Core::Task task;
    switch (command) {
        case Command::ADD:
            task.set_title("test");
            break;
        case Command::EDIT:
            task.set_title("edited");
            break;
        case Command::SUBTASK:
            task.set_title("sub");
            break;
        default:
            break;
    }

    switch (command) {
        case Command::GETALL:
            res = tm.getTasks();
            break;
        case Command::GET_BY_LABEL:
            res = tm.getTasks(getRandomLabel());
            break;
        case Command::GET_BY_ID:
            res = tm.getTaskWithSubtasks(getRandomID(tm));
            break;
        case Command::ADD:
            tm.Add(task);
            break;
        case Command::EDIT:
            tm.Edit(getRandomID(tm), task);
            break;
        case Command::SUBTASK:
            tm.AddSubtask(task, getRandomID(tm));
            break;
        case Command::COMPLETE:
            tm.Complete(getRandomID(tm));
            break;
        case Command::UNCOMPLETE:
            tm.Uncomplete(getRandomID(tm));
            break;
        case Command::DELETE:
            tm.Delete(getRandomID(tm), true);
            break;
        case Command::IS_PRESENT:
            tm.IsPresent(id);
            break;
        case Command::LABEL:
            tm.AddLabel(getRandomID(tm), getRandomLabel());
            break;
        case Command::UNLABEL:
            tm.RemoveLabel(getRandomID(tm), getRandomLabel());
            break;
        case Command::UNLABEL_ALL:
            tm.RemoveAllLabels(getRandomID(tm));
            break;
    }

    return res;
}

Command getRandomCommand() {
    std::uniform_int_distribution<int> uni(0, 12); // guaranteed unbiased
    auto random_integer = uni(rng);
    return (Command) random_integer;
}

void runRandomCommands(TaskManager &tm, int n) {
    for (int i = 0; i < n; ++i) {
        runCommand(getRandomCommand(), tm);
    }
}

void runRandomCommandsMultiThread(TaskManager &tm) {
    int num_threads = 5;
    int num_commands_per_thread = 1000;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(runRandomCommands, std::ref(tm), num_commands_per_thread);
    }

    for (auto &t : threads)
        t.join();
}

TEST_F(DISABLED_TaskManagerStressTest, shouldRunRandomCommandsWithMockedIDGenerator) {
    auto gen = std::make_shared<MockIDGenerator>();
    std::mutex id_mut;
    Core::TaskID id;
    id.set_value(0);
    EXPECT_CALL(*gen, genID)
        .WillRepeatedly(Invoke([&id, &id_mut]{
            std::lock_guard lk{id_mut};
            id.set_value(id.value()+1);
            return id;
        }));

    TaskManager tm{gen};
    runRandomCommandsMultiThread(tm);
}

TEST_F(DISABLED_TaskManagerStressTest, shouldRunRandomCommandsWithRealIDGenerator) {
    TaskManager tm;
    runRandomCommandsMultiThread(tm);
}
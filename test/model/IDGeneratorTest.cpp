//
// Created by Anton O. on 2/18/22.
//

#include <algorithm>
#include <thread>

#include "model/IDGenerator.h"
#include "utilities/TaskIDUtils.h"

#include <gtest/gtest.h>

class IDGeneratorTest : public ::testing::Test
{

};

TEST_F(IDGeneratorTest, shouldCeate100UniqueIDs)
{
    int N = 100;
    IDGenerator gen{1};
    std::vector<Core::TaskID> ids;
    ids.reserve(N);
    for (int i = 0; i < N; ++i)
        ids.emplace_back(gen.genID());

    auto it = std::unique(ids.begin(), ids.end());
    EXPECT_EQ(it, ids.end());
    EXPECT_EQ(N+1, gen.state());
}

void genIDs(std::vector<Core::TaskID> &ids, IDGenerator &generator, int num_ids) {
    for (int i = 0; i < num_ids; ++i)
        ids.emplace_back(generator.genID());
}

TEST_F(IDGeneratorTest, shouldWorkWithMultipleThreads)
{
    int num_threads = 10;
    int num_ids = 1000;
    IDGenerator gen{1};

    std::vector<std::thread> threads;
    std::vector<std::vector<Core::TaskID>> ids;
    threads.reserve(num_threads);
    ids.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        ids.emplace_back();
        threads.emplace_back(genIDs, std::ref(ids[i]), std::ref(gen), num_ids);
    }

    for (auto &t : threads)
        t.join();

    std::vector<Core::TaskID> all_ids;
    for (auto & id : ids)
        all_ids.insert(all_ids.end(), id.begin(), id.end());

    auto it = std::unique(all_ids.begin(), all_ids.end());
    EXPECT_EQ(it, all_ids.end());
    EXPECT_EQ(num_ids * num_threads + 1, gen.state());
}
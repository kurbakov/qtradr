#include <gtest/gtest.h>

#include <concurrency/lfqueue/ring/mpmc.hpp>

TEST(LFQueueRingMPMC, push)
{
    concurrency::lfqueue::ring::mpmc<int, 16> queue;

    EXPECT_TRUE(queue.push(1));
}

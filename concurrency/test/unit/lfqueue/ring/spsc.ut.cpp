#include <gtest/gtest.h>

#include <concurrency/lfqueue/ring/spsc.hpp>

TEST(LFQueueRingSPSC, push)
{
    concurrency::lfqueue::ring::spsc<int *, 2> queue;

    int *v = new int{0};
    EXPECT_TRUE(queue.push(v));
    delete v;
}

TEST(LFQueueRingSPSC, pushAndPop)
{
    concurrency::lfqueue::ring::spsc<int *, 16> queue;
    EXPECT_EQ(0, queue.size());
    EXPECT_TRUE(queue.empty());
    EXPECT_FALSE(queue.full());

    int *val = new int(0);
    EXPECT_TRUE(queue.push(val));
    EXPECT_EQ(1, queue.size());
    EXPECT_FALSE(queue.empty());
    EXPECT_FALSE(queue.full());

    int *res = nullptr;
    EXPECT_TRUE(queue.pop(res));
    EXPECT_EQ(0, queue.size());
    EXPECT_TRUE(queue.empty());
    EXPECT_FALSE(queue.full());

    EXPECT_EQ(val, res);
    delete val;
}

TEST(LFQueueRingSPSC, full)
{
    concurrency::lfqueue::ring::spsc<int *, 8> queue;

    int *val = new int(0);
    for (int i = 0; i < 8; ++i)
    {
        EXPECT_TRUE(queue.push(val));
    }

    EXPECT_TRUE(queue.full());

    EXPECT_FALSE(queue.push(val));

    delete val;
}

TEST(LFQueueRingSPSC, TwoThreads)
{
    concurrency::lfqueue::ring::spsc<int *, 64> queue;

    std::thread consumer(
        [&queue]()
        {
            int *recv_val = nullptr;
            while (true)
            {
                while (not queue.pop(recv_val))
                {
                    // busy wait
                }

                if (*recv_val == -1)
                {
                    break;
                }
            }
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    int *send_val = new int(1);
    while (*send_val < 512)
    {
        while (not queue.push(send_val))
        {
            // busy wait
        }

        *send_val += 1;
    }

    *send_val = -1;
    while (not queue.push(send_val))
    {
    }

    consumer.join();
}

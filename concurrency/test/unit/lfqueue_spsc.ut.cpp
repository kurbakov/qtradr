#include <gtest/gtest.h>

#include "concurrency/lfqueue/spsc.hpp"

TEST(LFQueueSPSC, construct)
{
    EXPECT_NO_THROW(concurrency::QueueSPSC<int*> queue);
}

TEST(LFQueueSPSC, push)
{
    concurrency::QueueSPSC<int*> queue;
    EXPECT_FALSE(queue.push(nullptr));

    int* v = new int{0};
    EXPECT_TRUE(queue.push(v));
    delete v;
}

TEST(LFQueueSPSC, pushAndPop)
{
    concurrency::QueueSPSC<int*> queue;
    EXPECT_EQ(0, queue.size());
    EXPECT_TRUE(queue.empty());
    EXPECT_FALSE(queue.full());

    int* val = new int(0);
    EXPECT_TRUE(queue.push(val));
    EXPECT_EQ(1, queue.size());
    EXPECT_FALSE(queue.empty());
    EXPECT_FALSE(queue.full());

    int* res = nullptr;
    EXPECT_TRUE(queue.pop(res));
    EXPECT_EQ(0, queue.size());
    EXPECT_TRUE(queue.empty());
    EXPECT_FALSE(queue.full());

    EXPECT_EQ(val, res);
    delete val;
}

TEST(LFQueueSPSC, full)
{
    concurrency::QueueSPSC<int*, 8> queue;
    EXPECT_EQ(8, queue.capacity());

    int* val = new int(0);
    for (int i=0; i<8; ++i)
    {
        EXPECT_TRUE(queue.push(val));
    }

    EXPECT_TRUE(queue.full());

    EXPECT_FALSE(queue.push(val));

    delete val;
}


TEST(LFQueueSPSC, TwoThreads)
{
    concurrency::QueueSPSC<int*, 64> queue;

    std::thread consumer ([&queue](){
        int* recv_val = nullptr;
        while(true)
        {
            while(not queue.pop(recv_val))
            {
                // busy wait
            }

            if(*recv_val == -1)
            {
                break;
            }
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    int* send_val = new int(1);
    while(*send_val < 512)
    {
        while(not queue.push(send_val))
        {
            // busy wait
        }

        *send_val += 1;
    }

    *send_val = -1;
    while(not queue.push(send_val)){}

    consumer.join();
}

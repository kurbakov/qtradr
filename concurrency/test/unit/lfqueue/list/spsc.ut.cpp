#include <concurrency/lfqueue/list/spsc.hpp>

#include <gtest/gtest.h>

TEST(LFQueueListSPSC, construct)
{
    EXPECT_NO_THROW(concurrency::lfqueue::list::QueueSPSC<int*> queue);
}

TEST(LFQueueListSPSC, push)
{
    concurrency::lfqueue::list::QueueSPSC<int*> queue;

    int* v = new int{0};
    EXPECT_TRUE(queue.push(v));
    delete v;
}

TEST(LFQueueListSPSC, pushAndPop)
{
    concurrency::lfqueue::list::QueueSPSC<int*> queue;
    EXPECT_EQ(0, queue.size());

    int* val = new int(0);
    EXPECT_TRUE(queue.push(val));
    EXPECT_EQ(1, queue.size());

    int* res = nullptr;
    EXPECT_TRUE(queue.pop(res));
    EXPECT_EQ(0, queue.size());

    EXPECT_EQ(val, res);
    delete val;
}

TEST(LFQueueListSPSC, TwoThreads)
{
    concurrency::lfqueue::list::QueueSPSC<int*> queue;

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

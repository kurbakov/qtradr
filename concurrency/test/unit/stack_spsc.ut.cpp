#include <concurrency/stack_spsc.hpp>

#include <iostream>
#include <thread>

#include <gtest/gtest.h>

TEST(StackSPSC, Empty)
{
    concurrency::stack<int> stack;

    int v;
    EXPECT_FALSE(stack.pop(v));
}

TEST(StackSPSC, Push)
{
    concurrency::stack<int> stack;

    int value = 1;
    EXPECT_TRUE(stack.push(value));
    EXPECT_TRUE(stack.pop(value));
}

TEST(StackSPSC, PushAndPop)
{
    concurrency::stack<int> stack;

    int expected = 1;
    int experiments = 1'000'000;
    for (int i = 0; i < experiments; ++i)
    {
        stack.push(expected);
    }

    for (int i = 0; i < experiments; ++i)
    {
        int value = 0;
        EXPECT_TRUE(stack.pop(value));
        EXPECT_EQ(expected, value);
    }
}

TEST(StackSPSC, PushAndPopTwoThreads)
{
    concurrency::stack<int> stack;

    int experiments = 100'000'000;
    std::thread consumer(
        [&stack, &experiments]()
        {
            for (int i = 0; i < experiments; ++i)
            {
                int received;
                while (!stack.pop(received))
                    ;
            }
        });

    std::thread producer(
        [&stack, &experiments]()
        {
            int expected = 1;
            for (int i = 0; i < experiments; ++i)
            {
                while (!stack.push(expected))
                    ;
            }
        });

    producer.join();
    consumer.join();
}

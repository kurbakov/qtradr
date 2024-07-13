#include <gtest/gtest.h>

#include <thread>

#include <timer/clock.hpp>

TEST(Clock, SecondTime)
{
    auto start = timer::second_time();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end = timer::second_time();
    EXPECT_TRUE(end > start);
    EXPECT_TRUE(start > 0);
}

TEST(Clock, MicrosecondTime)
{
    auto start = timer::microsecond_time();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end = timer::microsecond_time();
    EXPECT_TRUE(end.tv_sec > start.tv_sec) << "Clock must always go forward";
}

TEST(Clock, NanosecondTime)
{
    auto start = timer::nanosecond_time();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end = timer::nanosecond_time();
    EXPECT_TRUE(end.tv_sec > start.tv_sec) << "Clock must always go forward";
}

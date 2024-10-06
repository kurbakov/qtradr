#include <iostream>
#include <thread>
#include <vector>

#include "logger/logger.hpp"

int main()
{
    LOG_INIT(logger::Level::INFO, 1);

    LOGR_INFO("Hello from main thread!");
    std::thread t1(
        []()
        {
            for (int i = 0; i < 5; ++i)
            {
                LOGR_INFO("Hello from another thread {}", i);
            }
        });

    std::thread t2(
        []()
        {
            for (int i = 0; i < 5; ++i)
            {
                LOGR_INFO("Hello from another thread {}", i);
            }
        });

    t1.join();
    t2.join();

    return 0;
}
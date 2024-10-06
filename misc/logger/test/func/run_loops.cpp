#include <iostream>

#include "logger/logger.hpp"

int main()
{
    LOG_INIT(logger::Level::INFO, 1);

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            LOGR_INFO("Buy-buy {} {}", i, j);
        }
    }

    return 0;
}
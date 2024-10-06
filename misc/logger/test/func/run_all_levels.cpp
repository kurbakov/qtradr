#include <iostream>

#include "logger/logger.hpp"

int main()
{
    LOG_INIT(logger::Level::INFO, 1);

    LOGR_DEBUG("What amazing logger!");
    LOGR_INFO("What amazing logger!");
    LOGR_WARN("What amazing logger!");
    LOGR_ERROR("What amazing logger!");
    LOGR_FATAL("What amazing logger!");

    return 0;
}
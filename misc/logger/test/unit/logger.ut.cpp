#include <gtest/gtest.h>

#include <logger/defs.hpp>
#include <logger/logger.hpp>
#include <logger/processor.hpp>
#include <queue>

TEST(Logger, LogData)
{
    LOG_INIT(logger::Level::INFO, -1);

    LOGR_FATAL("hello {}", 0);
    LOGR_FATAL("hello");

    LOGR_ERROR("hello {}", 0);
    LOGR_ERROR("hello");

    LOGR_WARN("hello {}", 0);
    LOGR_WARN("hello");

    LOGR_INFO("hello {}", 0);
    LOGR_INFO("hello");

    LOGR_DEBUG("hello {}", 0);
    LOGR_DEBUG("hello");
}
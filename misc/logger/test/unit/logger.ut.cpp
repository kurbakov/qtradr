#include <gtest/gtest.h>

#include <logger/defs.hpp>
#include <logger/logger.hpp>
#include <logger/processor.hpp>
#include <queue>

TEST(Logger, LogData)
{
    logger::Processor<std::queue>::init(logger::Level::INFO);

    LOG_FATAL("hello {}", 0);
    LOG_FATAL("hello");

    LOG_ERROR("hello {}", 0);
    LOG_ERROR("hello");

    LOG_WARN("hello {}", 0);
    LOG_WARN("hello");

    LOG_INFO("hello {}", 0);
    LOG_INFO("hello");

    LOG_DEBUG("hello {}", 0);
    LOG_DEBUG("hello");
}
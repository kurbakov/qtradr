#include <gtest/gtest.h>

#include <logger/processor.hpp>
#include <logger/defs.hpp>
#include <logger/logger.hpp>
#include <queue>

template <typename T, size_t Size>
class FakeQueue : public std::queue<T> {

};

TEST(Logger, LogData) {
    logger::Processor<FakeQueue>::init(logger::Level::INFO);

    LOG_INFO("hello {} {}", 0, 1);
    LOG_WARN("hello {} {}", 0, 1);
    LOG_DEBUG("hello {} {}", 0, 1);
    LOG_ERROR("hello {} {}", 0, 1);
    LOG_FATAL("hello {} {}", 0, 1);
}
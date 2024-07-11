#pragma once

#include <chrono>
#include <cstdint>
#include <format>
#include <source_location>
#include <string>
#include <thread>

namespace logger
{

enum class Level : uint8_t
{
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
};

struct Data
{
    using clock = std::chrono::system_clock;
    using resolution = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<clock, resolution>;

    const std::thread::id thread_id;
    const time_point timestamp;
    const std::format_args args;
};

struct Meta
{
    const Level level;
    const std::source_location location;
    const std::string_view pattern;
};

} // namespace logger

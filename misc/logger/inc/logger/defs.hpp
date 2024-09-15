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
    const uint64_t meta_id;
    const std::thread::id thread_id;
    const timespec timestamp;
    const std::format_args args;
};

struct Meta
{
    const uint64_t id;
    const Level level;
    const std::source_location location;
    const std::string_view pattern;
};

} // namespace logger

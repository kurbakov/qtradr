#pragma once

#include <cstdint>
#include <source_location>
#include <string_view>
#include <sys/time.h>
#include <thread>

#include "concurrency/lfqueue/ring/spsc.hpp"
#include "memory/fix_size_pool.hpp"

namespace logger
{
struct Log;
struct Data;

static constexpr size_t ARGS_MAX_LEN = 1024;
using LogQueue = concurrency::lfqueue::ring::spsc<Log, ARGS_MAX_LEN>;
using LogDataPool = memory::FixSizePool<Data, ARGS_MAX_LEN>;

enum class Level : uint8_t
{
    INVALID = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    Last,
};

std::string_view to_string(Level level)
{

    std::array<std::string_view, static_cast<size_t>(Level::Last)> m_map{"INVALID", "DEBUG", "INFO",
                                                                         "WARN",    "ERROR", "FATAL"};

    return m_map[static_cast<size_t>(level)];
}

struct Data
{
    char args[ARGS_MAX_LEN];
    std::thread::id tid;
    timeval tv;
};

struct Meta
{
    Level level;
    std::source_location location;
    std::string_view pattern;
};

struct Log
{
    size_t uuid;
    const Data *data;
    const Meta *meta;

    Log() : uuid(0), data(nullptr), meta(nullptr) {}
    Log &operator=(const Log &other)
    {
        if (other != *this)
        {
            uuid = other.uuid;
            data = other.data;
            meta = other.meta;
        }

        return *this;
    }

    friend bool operator==(const Log &lhs, const Log &rhs)
    {
        return lhs.uuid == rhs.uuid && lhs.data == rhs.data && lhs.meta == rhs.meta;
    }
};
} // namespace logger

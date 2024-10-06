#pragma once

#include <ctime>
#include <iostream>
#include <memory.h>
#include <sys/time.h>

#include <sstream>

#include "logger/client.hpp"

namespace logger
{

consteval size_t placeholders_count(const std::string_view view, const std::string_view pattern)
{
    size_t res{0}, pos{0};
    while (pos = view.find(pattern, pos), pos != std::string_view::npos)
    {
        ++res;
        pos += pattern.size();
    }
    return res;
}

template <typename... Args> consteval size_t args_count(Args &&...args) { return sizeof...(args); }

inline __attribute__((always_inline)) timeval microsecond_time()
{
    timeval tv{};
    gettimeofday(&tv, nullptr);

    return tv;
}

template <typename... Args> void serialise(char *buffer, int len, Args &&...args)
{
    memset(buffer, 0, len);
    std::ostringstream ss;
    ss.rdbuf()->pubsetbuf(buffer, len);

    {
        ([&] { ss << "\"" << args << "\" "; }(), ...);
    }
    buffer[ss.view().length()] = '\n';
}

template <typename... Args> void process(const logger::Level level, const size_t id, Client &client, Args &&...args)
{
    if (client.get_level() > level)
        return;

    auto *data = client.allocate();

    data->tid = std::this_thread::get_id();
    data->tv = logger::microsecond_time();

    serialise(data->args, logger::ARGS_MAX_LEN, args...);

    client.push(id, data);
};

[[maybe_unused]] void process(const logger::Level level, const size_t id, Client &client, const logger::Meta *meta)
{
    if (client.get_level() > level)
        return;

    client.push(id, meta);
}

} // namespace logger
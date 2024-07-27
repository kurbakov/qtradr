#pragma once

#include <format>
#include <mutex>
#include <source_location>
#include <string_view>
#include <thread>

#include <concurrency/lfqueue/list/mpsc.hpp>
#include <timer/clock.hpp>

#include "defs.hpp"
#include "processor.hpp"
#include "utils.hpp"

namespace
{

using Processor = logger::Processor<concurrency::lfqueue::list::mpsc>;

template <typename Logger>
inline void log_meta(Logger *logger, const uint64_t id, const logger::Level &level,
                     const std::source_location &source_location, const std::string_view fmt)
{
    logger::Meta meta{
        .id = id,
        .level = level,
        .location = source_location,
        .pattern = fmt,
    };

    logger->write(std::move(meta));
}

template <typename Logger, typename... Args> inline void log_data(Logger *logger, uint64_t meta_id, Args &&...args)
{
    logger::Data data{
        .id = meta_id,
        .thread_id = std::this_thread::get_id(),
        .timestamp = timer::nanosecond_time(),
        .args = std::make_format_args(args...),
    };

    logger->write(std::move(data));
}

#define LOG_IMPL(log_level, logger_ptr, fmt, ...)                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == logger::args_count(__VA_ARGS__));                       \
        if (!logger_ptr || logger_ptr->level() < log_level)                                                            \
            return;                                                                                                    \
        static uint64_t meta_id = logger_ptr->get_id();                                                                \
        static std::once_flag flag;                                                                                    \
        std::call_once(flag, log_meta<Processor>, logger_ptr, meta_id, logger::Level::DEBUG,                           \
                       std::source_location::current(), fmt);                                                          \
        log_data<Processor>(logger_ptr, meta_id, ##__VA_ARGS__);                                                       \
    } while (0)

} // namespace

#define LOG_FATAL(fmt, ...) LOG_IMPL(logger::Level::FATAL, Processor::get(), fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_IMPL(logger::Level::ERROR, Processor::get(), fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_IMPL(logger::Level::WARN, Processor::get(), fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_IMPL(logger::Level::INFO, Processor::get(), fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG_IMPL(logger::Level::DEBUG, Processor::get(), fmt, ##__VA_ARGS__)

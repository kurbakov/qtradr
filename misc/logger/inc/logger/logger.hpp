#pragma once

#include <format>
#include <source_location>
#include <string_view>
#include <thread>

#include <concurrency/lfqueue/ring/mpmc.hpp>

#include "defs.hpp"
#include "utils.hpp"
#include "processor.hpp"

namespace
{

template <typename... Args> void LOG_IMPL(const logger::Meta &meta, Args &&...args)
{
    auto *logger_ptr = logger::Processor<concurrency::lfqueue::ring::mpmc>::get();
    if (logger_ptr == nullptr || logger_ptr->level() < meta.level)
        return;

    logger::Data data{
        .thread_id = std::this_thread::get_id(),
        .timestamp = std::chrono::system_clock::now(),
        .args = std::make_format_args(args...),
    };

    logger_ptr->write(meta, std::move(data));
}
} // namespace

#define LOG_DEBUG(fmt, ...)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == logger::args_count(__VA_ARGS__));                                       \
        constexpr logger::Meta meta{                                                                                   \
            .level{logger::Level::DEBUG},                                                                               \
            .location{std::source_location::current()},                                                                \
            .pattern{fmt},                                                                                             \
        };                                                                                                             \
        LOG_IMPL(meta, __VA_ARGS__);                                                                                   \
    } while (0)


#define LOG_INFO(fmt, ...)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == logger::args_count(__VA_ARGS__));                                       \
        constexpr logger::Meta meta{                                                                                   \
            .level{logger::Level::INFO},                                                                               \
            .location{std::source_location::current()},                                                                \
            .pattern{fmt},                                                                                             \
        };                                                                                                             \
        LOG_IMPL(meta, __VA_ARGS__);                                                                                   \
    } while (0)

#define LOG_WARN(fmt, ...)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == logger::args_count(__VA_ARGS__));                                       \
        constexpr logger::Meta meta{                                                                                   \
            .level{logger::Level::WARN},                                                                               \
            .location{std::source_location::current()},                                                                \
            .pattern{fmt},                                                                                             \
        };                                                                                                             \
        LOG_IMPL(meta, __VA_ARGS__);                                                                                   \
    } while (0)

#define LOG_ERROR(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == logger::args_count(__VA_ARGS__));                                       \
        constexpr logger::Meta meta{                                                                                   \
            .level{logger::Level::ERROR},                                                                               \
            .location{std::source_location::current()},                                                                \
            .pattern{fmt},                                                                                             \
        };                                                                                                             \
        LOG_IMPL(meta, __VA_ARGS__);                                                                                   \
    } while (0)

#define LOG_FATAL(fmt, ...)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == args_count(__VA_ARGS__));                                       \
        constexpr logger::Meta meta{                                                                                   \
            .level{logger::Level::FATAL},                                                                               \
            .location{std::source_location::current()},                                                                \
            .pattern{fmt},                                                                                             \
        };                                                                                                             \
        LOG_IMPL(meta, __VA_ARGS__);                                                                                   \
    } while (0)

#pragma once

#include <cstdint>
#include <format>
#include <source_location>
#include <thread>

#include "logger/client.hpp"
#include "logger/defs.hpp"
#include "logger/processor.hpp"
#include "logger/utils.hpp"

namespace
{
thread_local logger::Client client;
std::atomic<size_t> uuid{0};
} // namespace

void LOG_INIT(logger::Level level, int cpu_id = -1)
{
    logger::Processor::init(level, cpu_id);
    logger::Processor::get()->start();
}
/**
 * Raw log: no formatting, split meta information and runtime data
 *
 */
#define LOGR(log_level, fmt, ...)                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        static_assert(logger::placeholders_count(fmt, "{}") == logger::args_count(__VA_ARGS__));                       \
        constexpr static logger::Meta meta{                                                                            \
            .level = log_level,                                                                                        \
            .location = std::source_location::current(),                                                               \
            .pattern = std::string_view{fmt},                                                                          \
        };                                                                                                             \
        static size_t log_id = 0;                                                                                      \
        [[maybe_unused]] static bool once = [&]()                                                                      \
        {                                                                                                              \
            log_id = uuid.fetch_add(1, std::memory_order::relaxed);                                                    \
            process(log_level, log_id, client, &meta);                                                                 \
            return true;                                                                                               \
        }();                                                                                                           \
        process(log_level, log_id, client, ##__VA_ARGS__);                                                             \
    } while (0)

#define LOGR_DEBUG(fmt, ...) LOGR(logger::Level::DEBUG, fmt, ##__VA_ARGS__)
#define LOGR_INFO(fmt, ...) LOGR(logger::Level::INFO, fmt, ##__VA_ARGS__)
#define LOGR_WARN(fmt, ...) LOGR(logger::Level::WARN, fmt, ##__VA_ARGS__)
#define LOGR_ERROR(fmt, ...) LOGR(logger::Level::ERROR, fmt, ##__VA_ARGS__)
#define LOGR_FATAL(fmt, ...) LOGR(logger::Level::FATAL, fmt, ##__VA_ARGS__)

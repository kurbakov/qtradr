#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <sys/time.h>
#include <time.h>
#include <ostream>

/**
 * @file Clock helper functions
 *
 * For clock we set 2 types of clock:
 *   1. CPU clock: number of ticks on tje CPU. It is useful if we want to compute time difference.
 *   The clock is faster and preciser then std::chrono, but due to the high precision, it fits only
 *   to measure short time differences
 *
 *   2. Clock in seconds/milli/micro/nano since epoch
 */
namespace timer
{

/**
 * For implementation of other ARCH please see:
 *  https://github.com/google/benchmark/blob/v1.1.0/src/cycleclock.h
 */
inline __attribute__((always_inline)) uint64_t cpu_cycle_clock()
{
#if defined(__i386__)
    int64_t ret;
    __asm__ volatile("rdtsc" : "=A"(ret));
    return ret;
#elif defined(__x86_64__) || defined(__amd64__)
    uint64_t low, high;
    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return (high << 32) | low;
#endif

    return 0;
}

/*************************************************************
Time in seconds
*************************************************************/

// fastest way to get seconds
inline __attribute__((always_inline)) uint64_t second_time() { return time(nullptr); }

// fastest way to get microseconds
inline timeval microsecond_time()
{
    timeval tv{};
    gettimeofday(&tv, nullptr);

    return tv;
}

// fastest way to get nanoseconds
inline __attribute__((always_inline)) timespec nanosecond_time()
{
    timespec timestamp{};
    clock_gettime(CLOCK_MONOTONIC, &timestamp);

    return timestamp;
}

} // namespace timer
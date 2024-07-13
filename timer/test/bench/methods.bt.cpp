#include <benchmark/benchmark.h>

#include <sys/time.h>
#include <ctime>
#include <chrono>

static void BM_Chrono(benchmark::State& state) {
    for (auto _ : state)
    {
        auto ts = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(ts);
    }
}

static void BM_ChronoWithCast(benchmark::State& state) {
    for (auto _ : state)
    {
        auto ts = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(ts.time_since_epoch()).count();
        benchmark::DoNotOptimize(ts);
        benchmark::DoNotOptimize(duration);
    }
}

static void BM_Gettime(benchmark::State& state) {
    timespec timestamp{};
    for (auto _ : state)
    {
        auto ts = clock_gettime(CLOCK_MONOTONIC, &timestamp);
        benchmark::DoNotOptimize(ts);
    }
}


static void BM_Gettimeofday(benchmark::State& state) {
    timeval tv{};
    for (auto _ : state)
    {
        auto ts = gettimeofday(&tv, nullptr);
        benchmark::DoNotOptimize(ts);
    }
}

BENCHMARK(BM_Gettimeofday);
BENCHMARK(BM_Gettime);
BENCHMARK(BM_Chrono);
BENCHMARK(BM_ChronoWithCast);
#include <benchmark/benchmark.h>

#include <timer/clock.hpp>

static void BM_CpuCycleClock(benchmark::State& state) {
    for (auto _ : state)
    {
        auto ts = timer::cpu_cycle_clock();
        benchmark::DoNotOptimize(ts);
    }
}

static void BM_SecondTime(benchmark::State& state) {
    for (auto _ : state)
    {
        auto ts = timer::second_time();
        benchmark::DoNotOptimize(ts);
    }
}

static void BM_MicrosecondTime(benchmark::State& state) {
    for (auto _ : state)
    {
        auto ts = timer::microsecond_time();
        benchmark::DoNotOptimize(ts);
    }
}

static void BM_NanosecondTime(benchmark::State& state) {
    for (auto _ : state)
    {
        auto ts = timer::nanosecond_time();
        benchmark::DoNotOptimize(ts);
    }
}

BENCHMARK(BM_CpuCycleClock);
BENCHMARK(BM_SecondTime);
BENCHMARK(BM_MicrosecondTime);
BENCHMARK(BM_NanosecondTime);

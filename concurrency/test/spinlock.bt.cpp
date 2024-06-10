#include <benchmark/benchmark.h>

#include <thread>

#include "concurrency/Spinlock.hpp"

static void BM_Spinlock_v1(benchmark::State& state) {
    concurrency::SpinLock sl;
    for (auto _ : state)
    {
        std::thread t1([&sl](){
            for (int i = 0; i < 5000; ++i){
                sl.lock();
                sl.unlock();
            }
        });

        std::thread t2([&sl](){
            for (int i = 0; i < 5000; ++i){
                sl.lock();
                sl.unlock();
            }
        });

        t1.join();
        t2.join();
    }
}

BENCHMARK(BM_Spinlock_v1);

static void BM_Spinlock_v2(benchmark::State& state) {
    concurrency::SpinLock sl;
    for (auto _ : state)
    {
        std::thread t1([&sl](){
            for (int i = 0; i < 5000; ++i){
                while(! sl.try_lock()){}
                sl.unlock();
            }
        });

        std::thread t2([&sl](){
            for (int i = 0; i < 5000; ++i){
                while(! sl.try_lock()){}
                sl.unlock();
            }
        });

        t1.join();
        t2.join();
    }
}

BENCHMARK(BM_Spinlock_v2);


BENCHMARK_MAIN();

#include "concurrency/spinlock.hpp"

#include <time.h>


namespace concurrency
{

SpinLock::SpinLock() : _flag(false) {}

void SpinLock::lock() noexcept
{
    static constexpr timespec ns {0, 1};
    static constexpr int sleep_each_attempts = 8;

    for (int i=0; _flag.load(std::memory_order_relaxed) || _flag.exchange(true, std::memory_order_acquire); ++i)
    {
        if (i == sleep_each_attempts)
        {
            // Sleep to lat the scheduler understand that we are
            // waiting and let other tassks get into the execution

            // if we do not sleep, scheduler will assume we are very busy and
            // put us high in priority. As the result we will have to wait longer
            i=0;
            nanosleep(&ns, nullptr);
        }
    }
}

bool SpinLock::try_lock() noexcept
{
    // First do a relaxed load to check if lock is free in order to prevent
    // unnecessary cache misses if someone does while(!try_lock())
    return !_flag.load(std::memory_order_relaxed) &&
           !_flag.exchange(true, std::memory_order_acquire);
}

void SpinLock::unlock()
{
    _flag.store(false, std::memory_order_release);
}

} // ns consurrency

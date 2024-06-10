#include <gtest/gtest.h>

#include "concurrency/Spinlock.hpp"

TEST(Spimlock, LockUnlock)
{
    concurrency::SpinLock sl;
    EXPECT_TRUE(sl.try_lock());
    sl.unlock();
}

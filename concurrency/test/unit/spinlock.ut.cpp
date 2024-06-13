#include <gtest/gtest.h>

#include "concurrency/spinlock.hpp"

TEST(Spimlock, LockUnlock)
{
    concurrency::SpinLock sl;
    EXPECT_TRUE(sl.try_lock());
    sl.unlock();
}

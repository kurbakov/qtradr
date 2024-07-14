#include <gtest/gtest.h>

#include <concurrency/thread.hpp>

TEST(Thread, InvalidCpuId){
    concurrency::ThreadParams params {
        .threadName = "unit test",
        .cpuId = 100,
        .detach_state = PTHREAD_CREATE_JOINABLE,
        .task = {.execute = nullptr, .args = nullptr},
    };

    concurrency::Thread thread(params);
    EXPECT_EQ(-1, thread.init());
}

TEST(Thread, InvalidDetschState){
    concurrency::ThreadParams params {
        .threadName = "unit test",
        .cpuId = 1,
        .detach_state = ~PTHREAD_CREATE_JOINABLE,
        .task = {.execute = nullptr, .args = nullptr},
    };

    concurrency::Thread thread(params);
    EXPECT_EQ(-1, thread.init());
}

TEST(Thread, AddInSeparateThread)
{
    struct Context
    {
        int a;
        int b;
        int r;
    };

    Context context{.a = 10, .b = 11, .r = 0};

    std::function<void *(void *)> func = [](void *args) -> void *
    {
        auto *data = reinterpret_cast<Context *>(args);
        data->r = data->a + data->b;
        return args;
    };

    concurrency::Task task{
        .execute = &func,
        .args = &context,
    };

    concurrency::ThreadParams params {
        .threadName = "unit test",
        .cpuId = 1,
        .detach_state = PTHREAD_CREATE_JOINABLE,
        .task = task,
    };

    concurrency::Thread thread(params);
    EXPECT_EQ(0, thread.init());
    EXPECT_EQ(0, thread.run());

    thread.join(nullptr);

    EXPECT_EQ(context.r, context.a + context.b);
}

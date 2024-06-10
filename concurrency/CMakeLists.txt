add_library(concurrency STATIC
    src/Spinlock.cpp
)

target_include_directories(concurrency PUBLIC inc/)

include_directories(
    inc/
)

# Tests

# Benchmark
add_executable(spinlock_bt
    test/spinlock.bt.cpp
)
target_link_libraries(spinlock_bt PRIVATE
    benchmark::benchmark
    concurrency
)


# Unit tests
add_executable(concurrency_ut
    test/spinlock.ut.cpp
)
target_link_libraries(concurrency_ut PRIVATE
    GTest::gtest_main
    concurrency
)

include(GoogleTest)
gtest_discover_tests(concurrency_ut
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/
)

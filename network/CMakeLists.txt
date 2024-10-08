set(LIB_NAME network)
file(GLOB_RECURSE LIB_SRC "src/*.cpp")
add_library(${LIB_NAME} STATIC ${LIB_SRC})
include_directories(inc/)
target_include_directories(${LIB_NAME} PUBLIC inc/)
target_link_libraries(${LIB_NAME} ssl crypto)


# Tests: Benchmark
set(BENCH_TEST ${LIB_NAME}_bt)
file(GLOB_RECURSE BENCH_TEST_SRC "test/bench/*.bt.cpp")
add_executable(${BENCH_TEST} ${BENCH_TEST_SRC})

target_link_libraries(${BENCH_TEST} PRIVATE
        benchmark::benchmark_main
        ${LIB_NAME}
)


# Tests: Unit
set(UNIT_TEST ${LIB_NAME}_ut)
file(GLOB_RECURSE UNIT_TEST_SRC "test/unit/*.ut.cpp")
add_executable(${UNIT_TEST} ${UNIT_TEST_SRC})

target_link_libraries(${UNIT_TEST} PRIVATE
        GTest::gtest_main
        ${LIB_NAME}
)

include(GoogleTest)
gtest_discover_tests(${UNIT_TEST}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/
)

# Tests: Func
set(FUNC_TLS_TEST ${LIB_NAME}_tls_ft)
file(GLOB_RECURSE FUNC_TLS_TEST_SRC "test/func/tls.ft.cpp")
add_executable(${FUNC_TLS_TEST} ${FUNC_TLS_TEST_SRC})
target_link_libraries(${FUNC_TLS_TEST} PRIVATE ${LIB_NAME})
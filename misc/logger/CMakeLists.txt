# Library
set(LIB_NAME logger)
file(GLOB_RECURSE LIB_SRC "src/*.cpp")
add_library(${LIB_NAME} STATIC ${LIB_SRC})
include_directories(inc/)
target_include_directories(${LIB_NAME} PUBLIC inc/)
target_link_libraries(${LIB_NAME} concurrency timer memory)

# Tests

# Unit tests
set(UNIT_TEST ${LIB_NAME}_ut)
file(GLOB_RECURSE UNIT_TEST_SRC "test/unit/*.ut.cpp")
add_executable(${UNIT_TEST} ${UNIT_TEST_SRC})

target_link_libraries(${UNIT_TEST} PRIVATE
    GTest::gtest_main
    ${LIB_NAME}
)

include(GoogleTest)
gtest_discover_tests(${UNIT_TEST}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/unit/
)


# Functional tests
add_executable(LoggerFT_RunAllLevels test/func/run_all_levels.cpp)
target_link_libraries(LoggerFT_RunAllLevels PRIVATE  ${LIB_NAME})

add_executable(LoggerFT_RunLoops test/func/run_loops.cpp)
target_link_libraries(LoggerFT_RunLoops PRIVATE  ${LIB_NAME})

add_executable(LoggerFT_RunThreads test/func/run_threads.cpp)
target_link_libraries(LoggerFT_RunThreads PRIVATE  ${LIB_NAME})

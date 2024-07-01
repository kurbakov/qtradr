# Library
add_library(config STATIC
        src/property_holder.cpp
)
target_include_directories(config PUBLIC inc/)
include_directories(inc/)

# Tests

# Unit tests
add_executable(qconfig_ut
        test/unit/helper.ut.cpp
        test/unit/property.ut.cpp
        test/unit/property_holder.ut.cpp
)
target_link_libraries(qconfig_ut
        GTest::gtest_main
        config
)

include(GoogleTest)
gtest_discover_tests(qconfig_ut
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/unit/
)

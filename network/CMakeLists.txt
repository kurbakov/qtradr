add_library(network STATIC
    src/TcpServer.cpp
    src/TcpClient.cpp
    src/UdpServer.cpp
    src/UdpClient.cpp
)

target_include_directories(network PUBLIC inc/)

include_directories(
        inc/
)

# Tests

# Benchmark
add_executable(udp_bt test/udp.bt.cpp)
target_link_libraries(udp_bt PRIVATE
    benchmark::benchmark
    network
)


# Unit tests
add_executable(network_ut
        test/udp.ut.cpp
)
target_link_libraries(network_ut PRIVATE
        GTest::gtest_main
        network
)

include(GoogleTest)
gtest_discover_tests(network_ut
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/
)

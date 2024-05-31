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

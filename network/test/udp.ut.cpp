#include "network/udp/UdpServer.hpp"

#include <thread>
#include <gtest/gtest.h>


TEST(UDP, InitServer)
{
    network::UDPServer server(98989);
    EXPECT_EQ(0, server.init());

    // once server is initialized, it should fail on repetative init call
    EXPECT_EQ(-1, server.init());
}

TEST(UDP, DeinitServer)
{
    network::UDPServer server(98989);

    // we should fail on deinit before we initialize
    EXPECT_EQ(-1, server.deinit());

    // once we initialized, we should be able to deinitialize
    server.init();
    EXPECT_EQ(0, server.deinit());
}

TEST(UDP, SendRecieve)
{
    const char* msg = "hello";
    char buffer[8];

    std::thread server([&msg](){
        network::UDPServer server(98989);
    });

    std::thread client([&buffer](){});

    server.join();
    client.join();

    EXPECT_EQ(0, strncmp(msg, buffer, strlen(msg)));
}

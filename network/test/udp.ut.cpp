#include "network/udp/UdpServer.hpp"
#include "network/udp/UdpClient.hpp"

#include <thread>
#include <sys/socket.h>
#include <gtest/gtest.h>

#include <cstring>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

TEST(UDP, InitServer)
{
    network::UDPServer server(98989);
    EXPECT_EQ(0, server.init());

    // once server is initialized, it should fail on repetative init call
    EXPECT_EQ(-1, server.init());
    server.deinit();
}

TEST(UDP, DeinitServer)
{
    network::UDPServer server(98989);

    // we should not fail on deinit before we initialize
    EXPECT_EQ(0, server.deinit());

    // once we initialized, we should be able to deinitialize
    server.init();
    EXPECT_EQ(0, server.deinit());
}

TEST(UDP, InitClient)
{
    network::UDPClient client;
    EXPECT_EQ(0, client.init());

    // once server is initialized, it should fail on repetative init call
    EXPECT_EQ(-1, client.init());
}

TEST(UDP, ClientDisconnect)
{
    network::UDPClient client;

    // we should not fail on deinit before we initialize
    EXPECT_EQ(0, client.disconnect());

    // once we initialized, we should be able to deinitialize
    client.init();
    EXPECT_EQ(0, client.disconnect());
}

TEST(UDP, SendRecieve)
{
    // echo server
    std::thread server([](){
        sockaddr_in client_addr;
        char buffer[100];

        network::UDPServer server(98989);
        server.init();
        int rc = server.recv_data(buffer, 100, client_addr);
        server.send_data(buffer, rc, client_addr);
    });

    // give some time to start the server
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // make sure we get response exactly what we sent
    std::thread client([](){
        char buffer[100];
        std::string_view message{"Hello Server"};

        network::UDPClient client;
        client.init();
        client.try_connect("127.0.0.1", 98989);
        client.send_data(message.data(), message.length());

        int rc = client.recv_data(buffer, 100);
        buffer[rc] = '\0';

        EXPECT_EQ(0, strncmp(message.data(), buffer, message.length()));
    });

    server.join();
    client.join();
}

#include "network/tcp/client.hpp"
#include "network/tcp/server.hpp"

#include <gtest/gtest.h>
#include <sys/socket.h>
#include <thread>

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/types.h>

TEST(TCP, InitServer)
{
    network::tcp::TCPServer server(98989);
    EXPECT_EQ(0, server.init());

    // once server is initialized, it should fail on repetative init call
    EXPECT_EQ(-1, server.init());
    server.deinit();
}

TEST(TCP, DeinitServer)
{
    network::tcp::TCPServer server(98989);

    // we should not fail on deinit before we initialize
    EXPECT_EQ(0, server.deinit());

    // once we initialized, we should be able to deinitialize
    server.init();
    EXPECT_EQ(0, server.deinit());
}

TEST(TCP, InitClient)
{
    network::tcp::TCPClient client;
    EXPECT_EQ(0, client.init());

    // once server is initialized, it should fail on repetative init call
    EXPECT_EQ(-1, client.init());
}

TEST(TCP, ClientDisconnect)
{
    network::tcp::TCPClient client;

    // we should not fail on deinit before we initialize
    EXPECT_EQ(0, client.disconnect());

    // once we initialized, we should be able to deinitialize
    client.init();
    EXPECT_EQ(0, client.disconnect());
}

TEST(TCP, SendRecieve)
{
    // echo server
    std::thread server(
        []()
        {
            sockaddr_in client_addr;
            char buffer[100];

            network::tcp::TCPServer tcp_server(98989);
            tcp_server.init();

            int client_fd = tcp_server.accept(client_addr);
            int rc = tcp_server.recv_data(client_fd, buffer, 100);
            tcp_server.send_data(client_fd, buffer, rc);
        });

    // give some time to start the server
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // make sure we get response exactly what we sent
    std::thread client(
        []()
        {
            char buffer[100];
            std::string_view message{"Hello Server"};

            network::tcp::TCPClient tcp_client;
            tcp_client.init();
            tcp_client.try_connect_to_ip4("127.0.0.1", 98989);
            tcp_client.send_data(message.data(), message.length());

            int rc = tcp_client.recv_data(buffer, 100);
            buffer[rc] = '\0';

            EXPECT_EQ(0, strncmp(message.data(), buffer, message.length()));
        });

    server.join();
    client.join();
}

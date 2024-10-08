#include "network/mcast/recv.hpp"
#include "network/mcast/send.hpp"

#include <thread>

#include <gtest/gtest.h>

TEST(MCAST, SendRecv)
{
    const std::string msg{"Hello"};
    const int iter = 10;
    const int port = 94930;
    const std::string_view mcast_group{"239.255.255.250"};

    std::jthread subscriber_1(
        [&]()
        {
            network::mcast::Receiver reciever;
            reciever.init(port, mcast_group);
            char buffer[16];
            for (int i = 0; i < iter; ++i)
            {
                reciever.recv_data(buffer, 16);
                EXPECT_EQ(0, strncmp(msg.c_str(), buffer, msg.length()));
            }
        });

    std::jthread subscriber_2(
        [&]()
        {
            network::mcast::Receiver receiver;
            receiver.init(port, mcast_group);
            char buffer[16];
            for (int i = 0; i < iter; ++i)
            {
                receiver.recv_data(buffer, 16);
                EXPECT_EQ(0, strncmp(msg.c_str(), buffer, msg.length()));
            }
        });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::jthread producer(
        [&]()
        {
            network::mcast::Sender sender;
            sender.init(port, mcast_group);

            for (int i = 0; i < iter; ++i)
            {
                sender.send_data(msg.c_str(), msg.length());
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
}

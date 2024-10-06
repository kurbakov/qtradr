#include "network/tls/client.hpp"

#include <gtest/gtest.h>

TEST(TLS, InitClient)
{
    std::string host = "127.0.0.1";
    int port = 123456;

    network::tls::Client client(host, port);
    EXPECT_EQ(-1, client.try_connect());
}

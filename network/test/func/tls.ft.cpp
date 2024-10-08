#include <cassert>
#include <iostream>

#include <network/tls/client.hpp>

bool connect(const std::string host, int port)
{
    network::tls::Client client(host, port);

    bool result;
    if (0 == client.try_connect())
    {
        result = true;
        std::cout << "Successfully connected" << std::endl;
    }
    else
    {
        result = false;
        std::cerr << "Failed to connect to: " << host << ":" << port << std::endl;
    }

    return result;
}

int main()
{
    // Make sure we can open a socket to the Coinbase end points:
    // - WebSocket
    // - Rest API

    // More information:
    // https://docs.cdp.coinbase.com/advanced-trade/docs/welcome

    const int port = 443;
    assert(connect("advanced-trade-ws.coinbase.com", port));
    assert(connect("api.coinbase.com", port));

    return 0;
}
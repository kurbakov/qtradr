#pragma once

#include <cstddef>

namespace network
{
    class UDPClient
    {
    public:
        int try_connect();
        int disconnect();

        int send(const char* msg, size_t len);
        int recv(char* buffer, size_t len);

    private:
        int _fd;
    };
} // ns network

#pragma once

#include <cstddef>

namespace network
{

class BroadcastSender
{
public:
    int init(){}
    int deinit(){}

    int send_data(const char* msg, size_t len){}

private:
    int _fd;
};

} // ns network

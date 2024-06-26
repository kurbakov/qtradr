#pragma once

#include <arpa/inet.h>
#include <cstddef>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <memory.h>
#include <cstring>


namespace network
{

class McastSend
{
public:
    McastSend() : _fd{-1}, _addr{}
    {
    }

    ~McastSend()
    {
        deinit();
    }

    int init(int port, std::string_view mcast_group)
    {
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr(mcast_group.data());
        _addr.sin_port = htons(port);

        return 0;
    }

    int deinit()
    {
        int rc = 0;
        if (_fd > 0)
        {
            rc = close(_fd);
        }

        return rc;
    }

    int send_data(const char *msg, int size)
    {
        int total = 0;
        while (total < size)
        {
            int nbytes = sendto(_fd, &msg[total], size - total, 0, (struct sockaddr *)&_addr, sizeof(_addr));
            if (nbytes < 0)
            {
                std::cerr << __FILE__ << ":" << __LINE__ << " Failed to send data" << std::endl;
                std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
                return -1;
            }

            total += nbytes;
        }

        return total;
    }

private:
    int _fd;
    sockaddr_in _addr;
};

} // ns network

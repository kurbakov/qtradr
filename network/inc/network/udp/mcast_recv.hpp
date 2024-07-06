#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory.h>
#include <netinet/in.h>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

namespace network
{

// Read: https://www.tenouk.com/Module41c.html
class McastRecv
{
public:
    int init(int port, std::string_view mcast_group)
    {
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        // allow multiple sockets to use the same PORT number
        u_int yes = 1;
        if (-1 == setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Reusing ADDR failed" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;

            deinit();
            return -1;
        }

        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = htonl(INADDR_ANY);
        _addr.sin_port = htons(port);

        if (-1 == bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to bind to the port: " << port << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;

            deinit();
            return -1;
        }

        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr(mcast_group.data());
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (-1 == setsockopt(_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to set a sock option IP_ADD_MEMBERSHIP" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;

            deinit();
            return -1;
        }

        return 0;
    }

    int deinit()
    {
        int rc = 0;
        if (0 < _fd)
        {
            rc = close(_fd);
            _fd = -1;
        }

        return rc;
    }

    int recv_data(char *buff, int size) const
    {
        if (-1 == _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Invalid socket to recvfrom() call" << std::endl;
            return -1;
        }

        socklen_t addrlen = sizeof(_addr);
        auto *addr = const_cast<sockaddr_in *>(&_addr);
        return recvfrom(_fd, buff, size, 0, reinterpret_cast<sockaddr *>(addr), &addrlen);
    }

private:
    int _fd;
    sockaddr_in _addr;
};

} // namespace network

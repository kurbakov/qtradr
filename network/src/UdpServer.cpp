#include "network/udp/UdpServer.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>


namespace network
{
    UDPServer::UDPServer(int port) : _fd(-1), _port(port)
    {
    }

    UDPServer::~UDPServer()
    {
        if (0 > _fd)
        {
            deinit();
        }
    }

    int UDPServer::init()
    {
        if (0 < _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " FD is created, please run 'deinit()' to close it before you create a new socket" << std::endl;
            return -1;
        }

        _fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (-1 == _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(_port);
        server_addr.sin_family = AF_INET;

        if (-1 == bind(_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to bind to the port: " << _port << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        return 0;
    }

    int UDPServer::deinit()
    {
        if (-1 == _fd)
        {
            return 0;
        }

        if (-1 == close(_fd))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to close socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        // socket was successfully closed so we can assign -1 to it
        _fd = -1;

        return 0;
    }

    void UDPServer::make_nonblock()
    {
        if (-1 == fcntl(_fd, F_SETFL, O_NONBLOCK))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to set FD as non-blocking" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        }
    }

    int UDPServer::recv_data(char *buffer, size_t buff_len, sockaddr_in &client_addr)
    {
        if (-1 == _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Can't recv data on invalid file descriptor" << std::endl;
            return -1;
        }

        if (nullptr == buffer || 0 == buff_len)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Buffer can't be a NULL pointer or hve length of 0" << std::endl;
            return -1;
        }

        socklen_t client_len = sizeof(client_addr);
        ssize_t recv_bytes = recvfrom(_fd, buffer, buff_len, 0, (struct sockaddr *)&client_addr, &client_len);
        if (-1 == recv_bytes)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to read data" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        }

        return recv_bytes;
    }

    int UDPServer::send_data(const char *msg, size_t len, sockaddr_in &dest)
    {
        if (-1 == _fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Can't recv data on invalid file descriptor" << std::endl;
            return -1;
        }

        if (msg == nullptr || len == 0)
        {
            return 0;
        }

        int completed = 0;
        while (completed < len)
        {
            ssize_t sent = sendto(_fd, msg, len, 0, (struct sockaddr *)&dest, sizeof(dest));
            if (-1 == sent)
            {
                std::cerr << __FILE__ << ":" << __LINE__ << " Failed to read data" << std::endl;
                std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;

                return -1;
            }

            completed += sent;
        }

        return completed;
    }

} // ns network

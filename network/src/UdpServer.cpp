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

int UDPServer::init()
{
    if (-1 != _fd)
    {
        std::cerr << "FD is created, please run 'deinit()' to close it before you create a new socket" << std::endl;
        return -1;
    }

    _fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == _fd)
    {
        std::cerr << "Failed to create socket" << std::endl;
        std::cerr << "Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // make socket non blocking
    if (-1 == fcntl(_fd, F_SETFL, O_NONBLOCK))
    {
        std::cerr << "Failed to set FD as non-blocking" << std::endl;
        std::cerr << "Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(_port);
    server_addr.sin_family = AF_INET;

    if (-1 == bind(_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)))
    {
        std::cerr << "Failed to bind to the port: " << _port << std::endl;
        std::cerr << "Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

int UDPServer::deinit()
{
    if (-1 == _fd)
    {
        std::cerr << "Can't close negative file descriptor: " << _fd << std::endl;
        return -1;
    }

    if (-1 == close(_fd))
    {
        std::cerr << "Failed to close socket" << std::endl;
        std::cerr << "Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // socket was successfully closed so we can assign -1 to it
    _fd = -1;

    return 0;
}

int UDPServer::recvform(char* buffer, size_t len, sockaddr_storage* client_addr) const
{
    if (-1 == _fd)
    {
        std::cerr << "Can't recv data on invalid file descriptor" << std::endl;
        return -1;
    }

    socklen_t client_addrlen = sizeof(client_addr);
    ssize_t rc = ::recvfrom(_fd, buffer, len, 0, reinterpret_cast<sockaddr*>(client_addr), &client_addrlen);
    if (-1 == rc)
    {
        std::cerr << "Failed to read data" << std::endl;
        std::cerr << "Message: " << std::strerror(errno) << std::endl;
    }

    return rc;
}


int UDPServer::sendto(const char* msg, size_t len, const sockaddr* dest) const
{
    if (-1 == _fd)
    {
        std::cerr << "Can't recv data on invalid file descriptor" << std::endl;
        return -1;
    }

    ssize_t completed = 0;

    // make sure we send all
    while(completed < len)
    {
        ssize_t rc = ::sendto(_fd, reinterpret_cast<void*>(msg[completed]), len - completed, 0, dest, sizeof(dest));
        if (-1 == rc)
        {
            std::cerr << "Failed to read data" << std::endl;
            std::cerr << "Message: " << std::strerror(errno) << std::endl;

            return -1;
        }

        completed += rc;
    }

    return 0;
}

} // ns network

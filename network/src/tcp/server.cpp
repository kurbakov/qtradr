#include "network/tcp/server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>


namespace network::tcp
{

TCPServer::TCPServer(int port) : _port(port), _fd(-1) {}

TCPServer::~TCPServer()
{
    if (0 < _fd)
    {
        deinit();
    }
}


int TCPServer::init(int backlog)
{
    if (0 < _fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " FD is created, please run 'deinit()' to close it before you create a new socket" << std::endl;
        return -1;
    }

    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    if (-1 == listen(_fd, backlog))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to switch to passive socket" << _port << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

int TCPServer::deinit()
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


int TCPServer::accept(sockaddr_in& client_addr)
{
    socklen_t client_addr_len = sizeof(client_addr);
    int rc = ::accept(_fd, (sockaddr*) &client_addr, &client_addr_len);

    if (-1 == rc)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to accept connection" << _port << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
    }

    return rc;
}

void TCPServer::make_nonblock()
{
    if (-1 == fcntl(_fd, F_SETFL, O_NONBLOCK))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to set FD as non-blocking" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
    }
}

int TCPServer::recv_data(int client_fd, char *buffer, size_t buff_len)
{
    if (-1 == client_fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Can't recv data on invalid file descriptor" << std::endl;
        return -1;
    }

    if (nullptr == buffer || 0 == buff_len)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Buffer can't be a NULL pointer or hve length of 0" << std::endl;
        return -1;
    }

    ssize_t recv_bytes = recv(client_fd, buffer, buff_len, 0);
    if (-1 == recv_bytes)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to read data" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
    }

    return recv_bytes;
}

int TCPServer::send_data(int client_fd, const char *msg, size_t len)
{
    if (-1 == client_fd)
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
        ssize_t sent = send(client_fd, msg, len, 0);
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

} // ns network::tcp

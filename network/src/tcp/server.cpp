#include "network/tcp/server.hpp"

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace network::tcp
{

TCPServer::TCPServer(int port) : m_port(port), m_fd(-1) {}

TCPServer::~TCPServer()
{
    if (0 < m_fd)
    {
        deinit();
    }
}

int TCPServer::init(int backlog)
{
    if (0 < m_fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " FD is created, please run 'deinit()' to close it before you create a new socket" << std::endl;
        return -1;
    }

    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == m_fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(m_port);
    server_addr.sin_family = AF_INET;

    if (-1 == bind(m_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to bind to the port: " << m_port << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    if (-1 == listen(m_fd, backlog))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to switch to passive socket" << m_port << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

int TCPServer::deinit()
{
    if (-1 == m_fd)
    {
        return 0;
    }

    if (-1 == close(m_fd))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to close socket" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // socket was successfully closed so we can assign -1 to it
    m_fd = -1;

    return 0;
}

int TCPServer::accept(sockaddr_in &client_addr)
{
    socklen_t client_addr_len = sizeof(client_addr);
    int rc = ::accept(m_fd, (sockaddr *)&client_addr, &client_addr_len);

    if (-1 == rc)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to accept connection" << m_port << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
    }

    return rc;
}

void TCPServer::make_nonblock()
{
    if (-1 == fcntl(m_fd, F_SETFL, O_NONBLOCK))
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

    size_t completed = 0;
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

} // namespace network::tcp

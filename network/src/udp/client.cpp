#include "network/udp/client.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace network::udp
{
UDPClient::UDPClient() : m_fd(-1) {}
UDPClient::~UDPClient()
{
    if (0 < m_fd)
    {
        close(m_fd);
    }
}

int UDPClient::init()
{
    if (-1 != m_fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__
                  << " FD is created, please run 'disconnect()' to close it before you create a new socket"
                  << std::endl;
        return -1;
    }

    m_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == m_fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

int UDPClient::try_connect_to_ip4(std::string_view ip, int port)
{
    if (-1 == m_fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " FD is not initialised. Init socket before try to connect"
                  << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(ip.data());
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;

    if (-1 == connect(m_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to connect" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;

        close(m_fd);
        m_fd = -1;

        return -1;
    }

    return 0;
}

int UDPClient::disconnect()
{
    if (-1 == m_fd)
    {
        return 0;
    }

    if (0 != close(m_fd))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to close socket" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    m_fd = -1;

    return 0;
}

void UDPClient::make_nonblock()
{
    if (-1 == fcntl(m_fd, F_SETFL, O_NONBLOCK))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to set FD as non-blocking" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
    }
}

int UDPClient::send_data(const char *msg, size_t len)
{
    size_t total = 0;
    while (total < len)
    {
        int rc = sendto(m_fd, msg, len, 0, (sockaddr *)NULL, sizeof(sockaddr));
        if (-1 == rc)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to send data to server" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        total += rc;
    }

    return total;
}

int UDPClient::recv_data(char *buffer, size_t len)
{
    return recvfrom(m_fd, buffer, len, 0, (struct sockaddr *)NULL, NULL);
}
} // namespace network::udp

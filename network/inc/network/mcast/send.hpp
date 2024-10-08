#pragma once

#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

namespace network::mcast
{

class Sender
{
public:
    Sender() : m_fd{-1}, m_addr{} {}

    ~Sender() { deinit(); }

    int init(int port, std::string_view mcast_group)
    {
        m_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == m_fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = inet_addr(mcast_group.data());
        m_addr.sin_port = htons(port);

        return 0;
    }

    int deinit()
    {
        int rc = 0;
        if (m_fd > 0)
        {
            rc = close(m_fd);
            m_fd = -1;
        }

        return rc;
    }

    int send_data(const char *msg, int size)
    {
        int total = 0;
        while (total < size)
        {
            int nbytes = sendto(m_fd, &msg[total], size - total, 0, (struct sockaddr *)&m_addr, sizeof(m_addr));
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
    int m_fd;
    sockaddr_in m_addr;
};

} // namespace network::mcast

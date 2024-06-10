#include "network/tcp/TcpClient.hpp"

namespace network
{

TCPClient::TCPClient() : _fd(-1) {}

TCPClient::~TCPClient()
{
    if (0 < _fd)
    {
        close(_fd);
    }
}

int TCPClient::init()
{
    if(-1 != _fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " FD is created, please run 'disconnect()' to close it before you create a new socket" << std::endl;
        return -1;
    }

    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == _fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create socket" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}

int TCPClient::try_connect_to_ip4(std::string_view ip, int port)
{
    if (-1 == _fd)
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " FD is not initialised. Init socket before try to connect" << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(ip.data());
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;

    if (-1 == connect(_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to connect" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;

        close(_fd);
        _fd = -1;

        return -1;
    }

    return 0;
}

int TCPClient::disconnect()
{
    if (-1 == _fd)
    {
        return 0;
    }

    if (0 != close(_fd))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to close socket" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
        return -1;
    }

    _fd = -1;

    return 0;
}

void TCPClient::make_nonblock()
{
    if (-1 == fcntl(_fd, F_SETFL, O_NONBLOCK))
    {
        std::cerr << __FILE__ << ":" << __LINE__ << " Failed to set FD as non-blocking" << std::endl;
        std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
    }
}

int TCPClient::send_data(const char *msg, size_t len)
{
    size_t total = 0;
    while (total < len)
    {
        int rc = sendto(_fd, msg, len, 0, (sockaddr *)NULL, sizeof(sockaddr));
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

int TCPClient::recv_data(char *buffer, size_t len)
{
    return recvfrom(_fd, buffer, len, 0, (struct sockaddr *)NULL, NULL);
}

} // ns network

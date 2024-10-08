#pragma once

#include <cstddef>
#include <string_view>

namespace network::udp
{
class UDPClient
{
public:
    UDPClient();
    ~UDPClient();

    /**
     * @brief init
     *
     * Initialise the socket
     *
     * @return -1 on error 0 is success
     */
    int init();

    /**
     * @brief try_connect_to_ip
     *
     * Connect to the given IPv4 and port
     *
     * @param ip - IPv4 address to connect
     * @param port - port to connect
     * @return -1 on error 0 is success
     */
    int try_connect_to_ip4(std::string_view ip, int port);

    /**
     * @brief disconnect
     *
     * Close connection if socket is connected
     *
     * @return -1 on error 0 is success
     */
    int disconnect();

    /**
     * @brief make_nonblock
     *
     * Convert the socket to non-block socket
     */
    void make_nonblock();

    /**
     * @brief send_data
     *
     * Send data via connected socket
     *
     * @param msg - data to send
     * @param len - length of the data to send
     * @return -1 on error, if sucess - number of bytes was sent
     */
    int send_data(const char *msg, size_t len);

    /**
     * @brief recv_data
     *
     * Receive data from the connected socket
     *
     * @param buffer - buffer where the message can be written
     * @param len - length of buffer
     * @return -1 on error, if sucess - number of bytes was received
     */
    int recv_data(char *buffer, size_t len);

private:
    int m_fd;
};
} // namespace network::udp

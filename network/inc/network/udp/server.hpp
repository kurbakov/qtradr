#pragma once

#include <cstddef>

struct sockaddr_in;
struct sockaddr;

namespace network::udp
{

class UDPServer
{
public:
    explicit UDPServer(int port);

    ~UDPServer();

    // disable copy, move
    UDPServer(UDPServer &other) = delete;
    UDPServer &operator=(UDPServer &other) = delete;
    UDPServer(UDPServer &&other) = delete;
    UDPServer &operator=(UDPServer &&other) = delete;

    /**
     * @brief init
     *
     * Create a file descriptor and bind to the port.
     * After this function, server should be ready to start recieving messages.
     *
     * @return -1 on error and 0 on success
     */
    int init();

    /**
     * @brief deinit
     *
     * Close the file descriptor
     *
     * @return -1 on error and 0 on success
     */
    int deinit();

    void make_nonblock();

    /**
     * @brief recv_data - read data from a file descriptor
     *
     * Read data from a file descriptor and save the client address
     *
     * @param buffer: buffer to store the message from the client
     * @param len: length of the buffer
     * @param client_addr: holder to store the client address
     * @return length of message received from client
     */
    int recv_data(char *buffer, size_t buff_len, sockaddr_in &client_addr);

    /**
     * @brief send_data - send message to the client
     *
     * Send complete message to the provided client
     *
     * @param msg: message to send
     * @param len: length of the message
     * @param dest: address of the client
     * @return -1 if fails, 0 on success
     */
    int send_data(const char *msg, size_t len, sockaddr_in &dest);

private:
    const int m_port;
    int m_fd;
};

} // namespace network::udp

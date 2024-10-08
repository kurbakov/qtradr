#pragma once

#include <cstddef>

struct sockaddr_in;

namespace network::tcp
{
class TCPServer
{
public:
    /**
     * @brief TCPServer
     * @param port
     */
    explicit TCPServer(int port);

    /**
     * Destructor
     */
    ~TCPServer();

    /**
     * @brief init: Function to initialise the server
     * @param backlog: max length of a waiting queue for connection
     * @return: 0 on success, -1 if error
     */
    int init(int backlog = 64);

    /**
     * @brief deinit: close the socket of the server
     * @return: 0 on success, -1 if error
     */
    int deinit();

    /**
     * @brief accept
     * @param client_addr
     * @return
     */
    int accept(sockaddr_in &client_addr);

    /**
     * @brief make_nonblock: make the server socket non-blocking
     */
    void make_nonblock();

    /**
     * @brief recv_data: receive data from the connected to the client socket
     * @param client_fd: socket of the client
     * @param buffer: memory array where to write the data
     * @param buff_len: length of the memory array
     * @return: number of received bytes, or -1 on error
     */
    int recv_data(int client_fd, char *buffer, size_t buff_len);

    /**
     * @brief send_data: send data to the client connected socket
     * @param client_fd: socket of the client
     * @param msg: bytes array to send
     * @param len: length of the bytes array
     * @return: number of sent bytes, or -1 on error
     */
    int send_data(int client_fd, const char *msg, size_t len);

private:
    const int m_port;
    int m_fd;
};
} // namespace network::tcp

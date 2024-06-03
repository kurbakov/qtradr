#pragma once

#include <cstddef>

struct sockaddr_storage;
struct sockaddr;

namespace network
{
    class UDPServer
    {
    public:
        UDPServer(int port) : _fd(-1), _port(port)
        {
        }

        ~UDPServer()
        {
            if (0 > _fd)
            {
                deinit();
            }
        }

        // disable copy, move
        UDPServer(UDPServer& other) = delete;
        UDPServer& operator=(UDPServer& other) = delete;
        UDPServer(UDPServer&& other) = delete;
        UDPServer& operator=(UDPServer&& other) = delete;

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

        /**
         * @brief recvform - read data from a file descriptor
         *
         * Read data from a file descriptor and save the client address
         *
         * @param buffer: buffer to store the message from the client
         * @param len: length of the buffer
         * @param client_addr: holder to storee the client address
         * @return length of message recieved from client
         */
        int recvform(char* buffer, size_t len, sockaddr_storage* client_addr) const;

        /**
         * @brief sendto - send message to the client
         *
         * Send complete message to the provided client
         *
         * @param msg: message to send
         * @param len: length of the message
         * @param dest: address of the client
         * @return -1 if fails, 0 on success
         */
        int sendto(const char* msg, size_t len, const sockaddr* dest) const;

    private:
        int _fd;
        const int _port;
    };
} // ns network

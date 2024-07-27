#pragma once

#include <atomic>
#include <cstring>
#include <functional>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace multiplexing
{

class EPoll
{
public:
    EPoll() : m_epollfd(-1), m_cncfd(-1), m_running(false), m_context(nullptr), m_callback(nullptr) {}

    ~EPoll()
    {
        if (m_cncfd > 0)
        {
            close(m_cncfd);
        }

        if (m_epollfd > 0)
        {
            close(m_epollfd);
        }
    }

    int init();

    int add_sock(uint32_t events, int sock);

    void run();

    void set_context(void *context) { m_context = context; }

    void set_callback(std::function<void(int, int, void *)> *func) { m_callback = func; }

    void stop();

private:
    // epoll FD
    int m_epollfd;

    // command-and-control fd
    int m_cncfd;

    // epoll status
    std::atomic_bool m_running;

    // context for the callback function
    void *m_context;

    // callback with following args:
    // int event = event type
    // int fd = file descriptor
    // void* context = context tp pass to the function
    std::function<void(int, int, void *)> *m_callback;
};

} // namespace multiplexing

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

    int init()
    {
        m_epollfd = epoll_create1(0);
        if (-1 == m_epollfd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create epoll socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        m_cncfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == m_cncfd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create CnC socket for epoll" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        return 0;
    }

    int add_sock(uint32_t events, int sock)
    {
        if (-1 == sock)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Can't add invalid FD" << std::endl;
            return -1;
        }

        epoll_event event{};
        event.events = events;
        event.data.fd = sock;

        if (-1 == epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sock, &event))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to add socket to epoll" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        return 0;
    }

    void run()
    {
        if (m_running.load(std::memory_order_release))
        {
            return;
        }

        m_running.store(true, std::memory_order_acquire);

        constexpr uint8_t max_ev_cnt = 32;
        epoll_event events[max_ev_cnt];

        while (m_running.load(std::memory_order_release))
        {
            const int rcv_ev = epoll_wait(m_epollfd, events, max_ev_cnt, -1);
            if (rcv_ev == -1)
            {
                std::cerr << __FILE__ << ":" << __LINE__ << " Failed to add socket to epoll" << std::endl;
                std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
                break;
            }

            for (int i = 0; i < rcv_ev; ++i)
            {
                if (events[i].data.fd == m_cncfd) [[unlikely]]
                {
                    break;
                }

                if (nullptr != m_callback) [[likely]]
                {
                    (*m_callback)(events[i].events, events[i].data.fd, m_context);
                }
            }
        }
    }

    void set_context(void *context) { m_context = context; }

    void set_callback(std::function<void(int, int, void *)> *func) { m_callback = func; }

    void stop()
    {
        m_running.store(false, std::memory_order_acquire);
        char msg{'S'};
        write(m_cncfd, &msg, sizeof(msg));
    }

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

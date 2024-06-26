#pragma once

#include <atomic>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>

namespace multiplexing
{

class EPoll
{
public:
    EPoll() : _epollfd(-1) {}

    ~EPoll()
    {
        if (_cncfd > 0)
        {
            close(_cncfd);
        }

        if (_epollfd > 0)
        {
            close(_epollfd);
        }
    }

    int init()
    {
        _epollfd = epoll_create1(0);
        if (-1 == _epollfd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create epoll socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        _cncfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == _cncfd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create CnC socket for epoll" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        return 0;
    }

    int add_sock(uint32_t events, int sock)
    {
        if(-1 == sock)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Can't add invalid FD" << std::endl;
            return -1;
        }

        epoll_event event{};
        event.events = events;
        event.data.fd = sock;

        if (-1 == epoll_ctl(_epollfd, EPOLL_CTL_ADD, sock, &event))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to add socket to epoll" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        return 0;
    }

    void run()
    {
        if (_running.load(std::memory_order_release))
        {
            return;
        }

        _running.store(true, std::memory_order_acquire);

        constexpr uint8_t max_ev_cnt = 32;
        epoll_event events[max_ev_cnt];

        while (_running.load(std::memory_order_release))
        {
            const int rcv_ev = epoll_wait(_epollfd, events, max_ev_cnt, -1);
            if (rcv_ev == -1)
            {
                std::cerr << __FILE__ << ":" << __LINE__ << " Failed to add socket to epoll" << std::endl;
                std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
                break;
            }

            for (int i = 0; i < rcv_ev; ++i)
            {
                if (events[rcv_ev].data.fd == _cncfd) [[unlikely]]
                {
                    break;
                }

                // ToDo: add a handler for the FD event
            }
        }
    }

    void stop()
    {
        _running.store(false, std::memory_order_acquire);
        char msg{'S'};
        write(_cncfd, &msg, sizeof(msg));
    }

private:
    int _epollfd;
    int _cncfd; // command-and-control fd
    std::atomic_bool _running;
};

} // ns multiplexing

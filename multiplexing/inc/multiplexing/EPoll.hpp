#pragma once

#include <atomic>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <cstddef>

namespace multiplexing {

template <typename Socket>
concept EpollSocket = requires(Socket sock, int fd, char* buffer, size_t len)
{
    // socket has a static method to read data
    {Socket::read_data(fd, buffer, len)} -> std::same_as<size_t>;

    // socket has method to return file descriptor
    {sock.get_fd()} -> std::same_as<int>;
};


template <EpollSocket Socket>
class EPoll
{
public:
    EPoll() : _epollfd(-1) {}

    ~EPoll()
    {
        if (_epollfd > 0)
        {
            close(_epollfd);
        }
    }

    int init()
    {
        _epollfd = epoll_create1(0);
        if (_epollfd == -1)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Failed to create epoll socket" << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << " Message: " << std::strerror(errno) << std::endl;
            return -1;
        }

        return 0;
    }

    int add_sock(uint32_t events, const Socket& sock)
    {
        int fd = sock.get_fd();
        if(-1 == fd)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Can't add invalid FD" << std::endl;
            return -1;
        }

        epoll_event event{};
        event.events = events;
        event.data.fd = fd;

        if (-1 == epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event))
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
                // need to have memory provider where we will read data!
                Socket::read_data(events[rcv_ev].data.fd, nullptr, 0);
            }
        }
    }

    void stop()
    {
        _running.store(false, std::memory_order_acquire);
    }

private:
    int _epollfd;
    std::atomic_bool _running;
};

} // ns multiplexing

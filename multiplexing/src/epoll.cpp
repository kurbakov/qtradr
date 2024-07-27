#include "multiplexing/epoll.hpp"

namespace multiplexing
{

int EPoll::init()
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

int EPoll::add_sock(uint32_t events, int sock)
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

void EPoll::run()
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

void EPoll::stop()
{
    m_running.store(false, std::memory_order_acquire);
    char msg{'S'};
    write(m_cncfd, &msg, sizeof(msg));
}

} // namespace multiplexing
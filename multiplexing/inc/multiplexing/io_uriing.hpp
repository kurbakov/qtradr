#pragma once

#include <iostream>
#include <liburing.h>
#include <linux/io_uring.h>
#include <memory.h>

// https://developers.redhat.com/articles/2023/04/12/why-you-should-use-iouring-network-io

#define MAX_CONNECTIONS 4096
#define MAX_MESSAGE_LEN 2048
#define BUFFERS_COUNT MAX_CONNECTIONS

namespace multiplexing
{
class iouring
{
public:
    iouring() {}

    int init()
    {
        if (io_uring_queue_init(m_queue_depth, &m_ring, 0) != 0)
        {
            std::cout << "io_uring_queue_init" << std::endl;
            return -1;
        }

        return 0;
    }

    int add_fd(int fd) { return 0; }

    void run()
    {
        if (m_running)
        {
            return;
        }

        m_running = true;
        io_uring_cqe *cqe;
        while (m_running)
        {
            int ret = io_uring_wait_cqe(&m_ring, &cqe);

            // handle

            io_uring_cqe_seen(&m_ring, cqe);
        }
    }

private:
    static const int m_queue_depth = 64;
    struct io_uring_params m_params;
    struct io_uring m_ring;

    std::atomic_bool m_running;

    io_uring_sqe *sqe;
    int m_fd;
};
} // namespace multiplexing
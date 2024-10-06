#pragma once

#include <csignal>
#include <vector>

#include "concurrency/spinlock.hpp"
#include "concurrency/utils.hpp"
#include "logger/channel.hpp"
#include "logger/streams/io.hpp"

namespace logger
{
void *execute(void *);

class Processor final
{
private:
    Level m_level;
    int m_cpu_id;
    std::vector<Channel *> m_channels;
    pthread_t m_worker;
    std::atomic_bool m_running;
    concurrency::SpinLock m_spinlock;

    static Processor *m_instance;

    Processor(Level level, int cpu_id) : m_level{level}, m_cpu_id{cpu_id}, m_channels{}, m_worker{}, m_running(false)
    {
        m_channels.reserve(1024);
    }

public:
    Processor(const Processor &obj) = delete;
    ~Processor()
    {
        stop();
        delete m_instance;
        m_instance = nullptr;
    }

    void subscribe(Channel *client, Level &level)
    {
        level = m_level;
        {
            std::lock_guard<concurrency::SpinLock> guard(m_spinlock);
            m_channels.push_back(client);
        }
    }

    void unsubscribe(Channel *client)
    {
        for (size_t i = 0; i < m_channels.size(); ++i)
        {
            if (m_channels[i] == client)
            {
                std::lock_guard<concurrency::SpinLock> guard(m_spinlock);
                m_channels[i] = m_channels[m_channels.size() - 1];
                m_channels.pop_back();
                break;
            }
        }
    }

    static void init(Level level, int cpu_id = -1)
    {
        if (m_instance == nullptr)
        {
            m_instance = new Processor(level, cpu_id);
        }
    }

    void start()
    {
        m_running = true;
        pthread_create(&m_worker, nullptr, &execute, (void *)m_instance);
    }

    void stop()
    {
        m_running = false;
        pthread_kill(m_worker, 9);
    }

    static Processor *get() { return m_instance; }

    void run()
    {
        if (m_cpu_id >= 0)
        {
            concurrency::set_cpu_affinity(m_cpu_id);
        }

        Log log{};
        while (m_running)
        {
            std::lock_guard<concurrency::SpinLock> guard(m_spinlock);
            for (auto *channel : m_channels)
            {
                if (!channel->empty() && channel->recv(log))
                {
                    stream::IO::process(log);
                    channel->free(const_cast<Data *>(log.data));
                }
            }
        }
    }
};

Processor *Processor::m_instance = nullptr;

void *execute(void *processor)
{
    reinterpret_cast<Processor *>(processor)->run();
    return nullptr;
}
} // namespace logger
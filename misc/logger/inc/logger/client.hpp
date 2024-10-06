#pragma once

#include <thread>

#include "logger/channel.hpp"
#include "logger/processor.hpp"

namespace logger
{
class Client
{
public:
    Client() : m_level(Level::INVALID) { Processor::get()->subscribe(&m_channel, m_level); }

    ~Client()
    {
        int idx = 0;
        while (not m_channel.empty() && idx++ < 3)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Processor::get()->unsubscribe(&m_channel);
    }

    Data *allocate() { return m_channel.allocate(); }

    void free(Data *mem) { return m_channel.free(mem); }

    bool push(size_t id, const Meta *meta)
    {
        Log log;
        log.uuid = id;
        log.data = nullptr;
        log.meta = meta;

        return m_channel.send(log);
    }

    bool push(size_t id, const Data *data)
    {
        Log log;
        log.uuid = id;
        log.data = data;
        log.meta = nullptr;

        return m_channel.send(log);
    }

    [[nodiscard]] Level get_level() const { return m_level; }

private:
    Level m_level;
    Channel m_channel;
};

} // namespace logger
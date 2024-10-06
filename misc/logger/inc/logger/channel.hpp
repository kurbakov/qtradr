#pragma once

#include "logger/defs.hpp"

namespace logger
{
class Channel
{
public:
    Data *allocate() { return m_pool.allocate(); }

    void free(Data *mem) { return m_pool.free(mem); }

    bool send(Log &log) { return m_queue.push(log); }

    bool recv(Log &log) { return m_queue.pop(log); }

    [[nodiscard]] bool empty() const { return m_queue.empty(); }

private:
    LogQueue m_queue;
    LogDataPool m_pool;
};
} // namespace logger
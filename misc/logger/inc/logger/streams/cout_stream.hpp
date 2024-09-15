#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>

namespace logger
{

class CoutStream
{
public:
    CoutStream() : m_iter{0}, m_queue{} {}

    int64_t write(std::string_view msg)
    {
        m_queue[m_iter] = msg;
        ++m_iter;

        if (m_iter == m_queue_depth)
        {
            commit();
        }

        return 0;
    }

    int64_t commit()
    {
        for (size_t i = 0; i < m_iter; ++i)
        {
            std::cout.write(m_queue[i].data(), m_queue[i].length());
        }
        std::cout.flush();

        m_iter = 0;
        return 0;
    }

private:
    static constexpr size_t m_queue_depth = 1024;
    size_t m_iter;
    std::array<std::string_view, m_queue_depth> m_queue;
};
} // namespace logger
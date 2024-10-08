#pragma once

#include <array>
#include <atomic>
#include <cstdint>

#include <iostream>

namespace memory
{
template <typename T, uint64_t S> class FixSizePool
{
public:
    FixSizePool() : m_pool(), m_memory(), m_iter(0)
    {
        for (uint64_t i = 0; i < S; ++i)
        {
            m_pool[i].store(&m_memory[i], std::memory_order_relaxed);
        }
    }

    T *allocate()
    {
        for (uint64_t i = 0; i < S; ++i)
        {
            auto *val = m_pool[m_iter].load(std::memory_order_relaxed);
            if (val != nullptr && m_pool[m_iter].compare_exchange_weak(val, nullptr))
            {
                return val;
            }
            ++m_iter;
            m_iter &= m_mask;
        }

        return nullptr;
    }

    void free(T *val)
    {
        if (val == nullptr)
            return;

        int idx = val - &m_memory[0];
        m_pool[idx].store(val);
    }

private:
    std::array<std::atomic<T *>, S> m_pool;
    std::array<T, S> m_memory;
    size_t m_iter;
    static constexpr size_t m_mask = S - 1;
};
} // namespace memory

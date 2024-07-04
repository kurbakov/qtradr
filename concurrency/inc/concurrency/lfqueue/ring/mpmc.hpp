#pragma once

#include <array>
#include <atomic>
#include <cstddef>

#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

namespace concurrency::lfqueue::ring
{

template <typename T, size_t capacity> class mpmc final
{
private:
    struct cell_t
    {
        std::atomic_size_t _sequence;
        T _data;
    };

public:
    /**
     * Credentials to:
     *   https://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
     */
    mpmc() : _ring{}, _idx_mask(capacity - 1)
    {
        // make sure capacity is n**2
        assert((capacity >= 2) && ((capacity & (capacity - 1)) == 0));

        size_t i = 0;
        for (auto &cell : _ring)
        {
            cell._sequence.store(i++, std::memory_order_relaxed);
        }

        _push_idx.store(0, std::memory_order_relaxed);
        _pop_idx.store(0, std::memory_order_relaxed);
    }

    ~mpmc() = default;

    mpmc(mpmc const &) = delete;
    void operator=(mpmc const &) = delete;

    bool push(const T &data)
    {
        cell_t *cell;
        size_t idx = _push_idx.load(std::memory_order_relaxed);

        while (true)
        {
            cell = &_ring[idx & _idx_mask];
            size_t seq = cell->_sequence.load(std::memory_order_acquire);

            if (seq == idx)
            {
                if (_push_idx.compare_exchange_weak(idx, idx + 1, std::memory_order_relaxed))
                {
                    break;
                }
            }
            else if (seq < idx)
            {
                return false;
            }
            else
            {
                idx = _push_idx.load(std::memory_order_relaxed);
            }
        }

        cell->_data = data;
        cell->_sequence.store(idx + 1, std::memory_order_release);

        return true;
    }

    bool pop(T &data)
    {
        cell_t *cell;
        size_t idx = _pop_idx.load(std::memory_order_relaxed);

        while (true)
        {
            cell = &_ring[idx & _idx_mask];
            size_t seq = cell->_sequence.load(std::memory_order_acquire);
            intptr_t dif = (intptr_t)seq - (intptr_t)(idx + 1);

            if (dif == 0)
            {
                if (_pop_idx.compare_exchange_weak(idx, idx + 1, std::memory_order_relaxed))
                {
                    break;
                }
            }
            else if (dif < 0)
            {
                return false;
            }
            else
            {
                idx = _pop_idx.load(std::memory_order_relaxed);
            }
        }

        data = cell->data_;
        cell->_sequence.store(idx + _idx_mask + 1, std::memory_order_release);

        return true;
    }

private:
    char _pad0[hardware_constructive_interference_size];

    std::array<cell_t, capacity> _ring;
    const size_t _idx_mask;

    char _pad1[hardware_constructive_interference_size];
    std::atomic_size_t _push_idx;

    char _pad2[hardware_constructive_interference_size];
    std::atomic_size_t _pop_idx;

    char _pad3[hardware_constructive_interference_size];
};

} // namespace concurrency::lfqueue::ring

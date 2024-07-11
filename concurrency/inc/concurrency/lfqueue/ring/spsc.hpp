#pragma once

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>

namespace concurrency::lfqueue::ring
{

// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
constexpr std::size_t cacheline = 64;

/**
 * @brief The lock free queue with single producre and single consumer.
 * Queue is using a ring buffer under the hood. The max number of elements
 * in the queue is limited by size of the ring buffer.
 */
template <typename T, size_t capacity> class spsc
{
    /**
     * Single producer - single consumer queue.
     * Based on: https://www.youtube.com/watch?v=K3P_Lmq6pw0
     * Code: https://github.com/CharlesFrasch/cppcon2023/tree/main
     * Slides:
     * https://github.com/CppCon/CppCon2023/blob/main/Presentations/SPSC_Lock-free_Wait-Free_Fifo_from_the_Ground_Up_CppCon_2023.pdf
     */
public:
    spsc() : _ring{}, _idx_mask(capacity - 1)
    {
        // make sure capacity is n**2
        assert((capacity >= 2) && ((capacity & (capacity - 1)) == 0));

        _push_idx.store(0, std::memory_order_relaxed);
        _pop_idx.store(0, std::memory_order_relaxed);
    }

    ~spsc() = default;

    spsc(spsc &other) = delete;
    spsc &operator=(spsc &other) = delete;

    size_t size() const noexcept
    {
        size_t push_idx = _push_idx.load(std::memory_order_relaxed);
        size_t pop_idx = _pop_idx.load(std::memory_order_relaxed);

        return push_idx - pop_idx;
    }

    bool empty() const noexcept { return size() == 0; }

    bool full() const noexcept { return size() == capacity; }

    bool push(const T &value)
    {
        size_t push_idx = _push_idx.load(std::memory_order_relaxed);
        if (full(push_idx, _cached_pop_idx))
        {
            _cached_pop_idx = _pop_idx.load(std::memory_order_acquire);

            if (full(push_idx, _cached_pop_idx))
            {
                return false;
            }
        }

        _ring[push_idx & _idx_mask] = value;
        _push_idx.store(push_idx + 1, std::memory_order_release);

        return true;
    }

    bool pop(T &value)
    {
        size_t pop_idx = _pop_idx.load(std::memory_order_relaxed);

        if (empty(_cached_push_idx, pop_idx))
        {
            _cached_push_idx = _push_idx.load(std::memory_order_acquire);
            if (empty(_cached_push_idx, pop_idx))
            {
                return false;
            }
        }

        value = _ring[pop_idx & _idx_mask];
        _pop_idx.store(pop_idx + 1, std::memory_order_release);

        return true;
    }

private:
    bool full(size_t push_idx, size_t pull_idx) const noexcept { return (push_idx - pull_idx) == capacity; }

    static bool empty(size_t push_idx, size_t pull_idx) noexcept { return push_idx == pull_idx; }

private:
    std::array<T, capacity> _ring;
    const size_t _idx_mask;

    alignas(cacheline) std::atomic_size_t _push_idx;
    alignas(cacheline) std::atomic_size_t _cached_push_idx;

    alignas(cacheline) std::atomic_size_t _pop_idx;
    alignas(cacheline) std::atomic_size_t _cached_pop_idx;
    char _padding[cacheline];
};

} // namespace concurrency::lfqueue::ring

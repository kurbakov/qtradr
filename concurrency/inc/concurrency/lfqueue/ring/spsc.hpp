#pragma once

#include <array>
#include <atomic>
#include <cstddef>

namespace concurrency::lfqueue::ring
{

#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else \
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

/**
 * @brief The lock free queue with single producre and single consumer.
 * Queue is using a ring buffer under the hood. The max number of elements
 * in the queue is limited by size of the ring buffer.
 */
template <typename T, size_t Capacity = 1024>
class QueueSPSC
{
    /**
     * Single producer - single consumer queue.
     * Based on: https://www.youtube.com/watch?v=K3P_Lmq6pw0
     * Code: https://github.com/CharlesFrasch/cppcon2023/tree/main
     * Slides: https://github.com/CppCon/CppCon2023/blob/main/Presentations/SPSC_Lock-free_Wait-Free_Fifo_from_the_Ground_Up_CppCon_2023.pdf
     */
public:
    QueueSPSC() : _push_idx(0), _pop_idx(0), _ring{}
    {
        static_assert(std::atomic_size_t::is_always_lock_free);
    }

    size_t capacity() const noexcept
    {
        return Capacity;
    }

    size_t size() const noexcept
    {
        size_t push_idx =_push_idx.load(std::memory_order_relaxed);
        size_t pop_idx = _pop_idx.load(std::memory_order_relaxed);

        return push_idx - pop_idx;
    }

    bool empty() const noexcept
    {
        return size() == 0;
    }

    bool full() const noexcept
    {
        return size() == capacity();
    }

    bool push(const T& value)
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

        _ring[push_idx % Capacity] = value;
        _push_idx.store(push_idx + 1, std::memory_order_release);
        return true;
    }

    bool pop(T& value)
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

        value = _ring[pop_idx % Capacity];
        _pop_idx.store(pop_idx + 1, std::memory_order_release);
        return true;
    }

private:
    bool full(size_t push_idx, size_t pull_idx) const noexcept {
        return (push_idx - pull_idx) == Capacity;
    }

    static bool empty(size_t push_idx, size_t pull_idx) noexcept {
        return push_idx == pull_idx;
    }

private:
    std::array<T, Capacity> _ring;

    alignas(hardware_destructive_interference_size) std::atomic_size_t _push_idx;
    alignas(hardware_destructive_interference_size) std::atomic_size_t _cached_push_idx;

    alignas(hardware_destructive_interference_size) std::atomic_size_t _pop_idx;
    alignas(hardware_destructive_interference_size) std::atomic_size_t _cached_pop_idx;
    char _padding[hardware_destructive_interference_size];
};

} // ns concurrency::lfqueue::ring

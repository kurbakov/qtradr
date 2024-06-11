#pragma once

#include <array>
#include <atomic>
#include <new>

namespace concurrency
{

#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else \
    // 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

template<typename T>
concept Pointer = std::is_pointer_v<T>;

/**
 * @brief The lock free queue with single producre and single consumer.
 * Queue is using a ring buffer under the hood. The max number of elements
 * in the queue is limited by size of the ring buffer.
 */
template<Pointer ObjectPtr, size_t Capacity = 1024>
class QueueSPSC
{
public:
    QueueSPSC() : _push_idx(0), _pop_idx(0), _ring{}
    {
        static_assert(std::atomic_size_t::is_always_lock_free);
    }

    size_t capacity() const
    {
        return Capacity;
    }

    size_t size()
    {
        return _push_idx - _pop_idx;
    }

    bool empty() const
    {
        return _push_idx == _pop_idx;
    }

    bool full() const
    {
        return (_push_idx - _pop_idx) == Capacity;
    }

    bool push(ObjectPtr obj)
    {}

    bool pull(ObjectPtr& obj)
    {}

private:
    std::array<ObjectPtr, Capacity> _ring;

    alignas(hardware_destructive_interference_size) std::atomic_size_t _push_idx;
    alignas(hardware_destructive_interference_size) std::atomic_size_t _cached_push_idx;

    alignas(hardware_destructive_interference_size) std::atomic_size_t _pop_idx;
    alignas(hardware_destructive_interference_size) std::atomic_size_t _cached_pop_idx;
    char _padding[hardware_destructive_interference_size];
};
} // ns concurrency

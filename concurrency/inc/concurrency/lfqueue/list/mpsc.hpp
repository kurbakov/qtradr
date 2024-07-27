#pragma once

namespace concurrency::lfqueue::list
{
#include <assert.h>
#include <atomic>

// source: https://github.com/chronoxor/CppBenchmark/blob/master/examples/lockfree/mpsc-queue.hpp
template <typename T> class mpsc
{
public:
    mpsc() : _head(new node_t), _tail(_head.load(std::memory_order_relaxed))
    {
        node_t *front = _head.load(std::memory_order_relaxed);
        front->next.store(NULL, std::memory_order_relaxed);
    }

    mpsc(const mpsc &) = delete;
    void operator=(const mpsc &) = delete;

    ~mpsc()
    {
        T output;
        while (this->dequeue(output))
        {
        }
        node_t *front = _head.load(std::memory_order_relaxed);
        delete front;
    }

    void enqueue(const T &input)
    {
        node_t *node = new node_t;
        node->data = input;
        node->next.store(NULL, std::memory_order_relaxed);

        node_t *prev_head = _head.exchange(node, std::memory_order_acq_rel);
        prev_head->next.store(node, std::memory_order_release);
    }

    bool dequeue(T &output)
    {
        node_t *tail = _tail.load(std::memory_order_relaxed);
        node_t *next = tail->next.load(std::memory_order_acquire);

        if (next == NULL)
        {
            return false;
        }

        output = next->data;
        _tail.store(next, std::memory_order_release);
        delete tail;
        return true;
    }

private:
    struct node_t
    {
        T data;
        std::atomic<node_t *> next;
    };

    typedef char cache_line_pad_t[64];

    cache_line_pad_t _pad0;
    std::atomic<node_t *> _head;

    cache_line_pad_t _pad1;
    std::atomic<node_t *> _tail;
};
} // namespace concurrency

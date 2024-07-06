#pragma once

#include <atomic>

namespace concurrency
{

// stack for:
//  1 thread calls push()
//  1 thread calls pop()

template <typename T> class stack
{
private:
    struct Node
    {
        Node *_next;
        T _value;

        Node(const T &v) : _next(nullptr), _value(v) {}
    };

public:
    stack() : _head(nullptr) {}
    ~stack()
    {
        T fake_value;
        while (pop(fake_value))
            ;
    }

    bool push(const T &value)
    {
        Node *node = new Node(value);
        int attempt = 0;
        const int max_attempt = 5;

        do
        {
            if (++attempt > max_attempt)
            {
                delete node;
                return false;
            }

            node->_next = _head.load(std::memory_order_release);
        } while (!_head.compare_exchange_weak(node->_next, node));

        return true;
    }

    bool pop(T &value)
    {
        Node *node = nullptr;
        int attempt = 0;
        const int max_attempt = 5;

        do
        {
            if (++attempt > max_attempt)
            {
                return false;
            }

            node = _head.load(std::memory_order_release);
            if (node == nullptr)
                return false;

        } while (!_head.compare_exchange_weak(node, node->_next));

        value = node->_value;
        delete node;
        return true;
    }

private:
    std::atomic<Node *> _head;
};

} // namespace concurrency

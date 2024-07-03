#pragma once

#include <atomic>
#include <memory>

namespace concurrency::lfqueue::list
{

// ToDo:
// 1. compare performance wit:
//   https://github.com/chronoxor/CppBenchmark/blob/master/examples/lockfree/spsc-queue.hpp


/**
 * @brief The QueueSPSC class
 */
template <typename T>
class QueueSPSC
{
public:
    QueueSPSC()
        : _head(new Node)
        , _tail(_head.load())
    {
    }

    QueueSPSC(QueueSPSC& other) = delete;
    QueueSPSC& operator=(QueueSPSC &other) = delete;

    ~QueueSPSC()
    {
        while(auto* old = _head.load())
        {
            _head.store(old->_next);
            delete old;
        }
    }

    bool push(const T& data)
    {
        Node* new_node = new Node;

        Node* old_tail = _tail.load();
        old_tail->_data = data;
        old_tail->_next = new_node;
        _tail.store(new_node);
        ++_size;

        return true;
    }

    bool pop(T& value)
    {
        auto* node = pop_head();
        if(node == nullptr)
        {
            return false;
        }
        value = node->_data;
        delete node;
        --_size;
        return true;
    }

    size_t size() const
    {
        return _size.load();
    }

private:
    struct Node
    {
        Node* _next;
        T _data;

        Node() : _next(nullptr), _data(nullptr) {}
    };

    Node* pop_head()
    {
        Node* old_head = _head.load();
        if (old_head == _tail.load())
        {
            return nullptr;
        }
        _head.store(old_head->_next);
        return old_head;
    }

    std::atomic<Node*> _head;
    std::atomic<Node*> _tail;
    std::atomic_size_t _size;
};

} // ns concurrency

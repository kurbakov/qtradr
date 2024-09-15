#pragma once

#include <cstdint>

/**
 * @file: Please for thread safe structures refer to concurrency lib
 */

namespace dsa
{

template <typename T> class Queue
{
public:
    Queue() : head(nullptr), tail(nullptr), qsize(0) {}
    ~Queue()
    {
        T val;
        // clear the queue and free the memory
        while (pop(val))
        {
        }
    }

    bool push(const T &val)
    {
        QNode<T> *node = new QNode(val);
        if (tail == nullptr)
        {
            tail = node;
            head = node;
        }
        else
        {
            tail->prev = node;
            tail = node;
        }

        ++qsize;

        return true;
    }

    bool pop(T &val)
    {
        if (head == nullptr)
        {
            return false;
        }

        auto *node = head;
        head = head->prev;

        if (head == nullptr)
        {
            tail = nullptr;
        }

        val = node->val;
        delete node;

        return true;
    }

    uint64_t size() const { return qsize; }

    bool empty() const { return size() == 0; }

private:
    template <class V> struct QNode
    {
        QNode *prev;
        V val;
    };

    QNode<T> *head;
    QNode<T> *tail;
    uint64_t qsize;
};

} // namespace dsa
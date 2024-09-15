#pragma once

#include <cstdint>

namespace dsa
{

template <class T> class SingleLinkedList
{
public:
    SingleLinkedList() : head(nullptr), tail(nullptr), lisl_size(0) {}
    ~SingleLinkedList() { clear(); }

    void push_back(const T &val)
    {
        auto *node = new SLListNode(val);
        if (head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }

        ++lisl_size;
    }

    bool pop_front(T &val)
    {
        if (head == nullptr)
            return false;

        val = head->val;
        auto *n = head;
        head = head->next;
        if (head == nullptr)
        {
            tail = nullptr;
        }
        delete n;
        --lisl_size;

        return true;
    }

    void clear()
    {
        T val;

        // free mem
        while (pop_front(val))
        {
        }
    }

    [[nodiscard]] uint64_t size() const { return lisl_size; }
    [[nodiscard]] bool empty() const { return size() == 0; }

private:
    struct SLListNode
    {
        SLListNode *next;
        T val;

        explicit SLListNode(const T &v) : next(nullptr), val(v) {}
    };

    SLListNode *head;
    SLListNode *tail;
    uint64_t lisl_size;
};

template <typename T> class DoubleLinkedList
{
public:
private:
    struct DLListNode
    {
        DLListNode *next;
        DLListNode *prev;
        T val;

        explicit DLListNode(T &v) : next(nullptr), prev(nullptr), val(v) {}
    };
};

} // namespace dsa

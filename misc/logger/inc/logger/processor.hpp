#pragma once

#include "defs.hpp"

#include <cstdint>
#include <signal.h>

namespace logger
{

static void *process(void *)
{
    // todo
    return nullptr;
}

template <template <typename T> class MPSC_LF_Queue> class Processor final
{
    union Event
    {
        Meta meta;
        Data data;
    };

    static Processor *m_instance;
    static uint64_t m_meta_id;
    MPSC_LF_Queue<Event> m_queue;
    const Level m_level;

    pthread_t m_consumer_thread;

    explicit Processor(Level level) : m_queue{}, m_level(level)
    {
        pthread_create(&m_consumer_thread, nullptr, &process, m_instance);
    }

    ~Processor()
    {
        pthread_kill(m_consumer_thread, 9);
        delete m_instance;
        m_instance = nullptr;
    }

public:
    Processor(const Processor &obj) = delete;

    [[nodiscard]] Level level() const { return m_level; }

    static void init(Level level)
    {
        if (m_instance == nullptr)
        {
            m_instance = new Processor(level);
        }

    }

    static Processor *get() { return m_instance; }

    uint64_t get_id() { return m_meta_id++; }

    void write(Meta &&) {}

    void write(Data &&) {}
};

template <template <typename T> class MPSC_LF_Queue>
Processor<MPSC_LF_Queue> *Processor<MPSC_LF_Queue>::m_instance = nullptr;

template <template <typename T> class MPSC_LF_Queue> uint64_t Processor<MPSC_LF_Queue>::m_meta_id = 0;

} // namespace logger
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

template <template <class T, std::size_t> class q> class Processor final
{
    struct Event
    {
        const Meta _meta;
        const Data _data;
    };

    static Processor *_instance;
    q<Event, 1024> _queue;
    const Level _level;

    pthread_t _consumer_thread;

    explicit Processor(Level level) : _queue{}, _level(level)
    {
        pthread_create(&_consumer_thread, nullptr, &process, _instance);
    }

    ~Processor() { pthread_kill(_consumer_thread, 9); }

public:
    Processor(const Processor &obj) = delete;

    [[nodiscard]] Level level() const { return _level; }

    static void init(Level level)
    {
        if (_instance == nullptr)
        {
            _instance = new Processor(level);
        }
    }

    static Processor *get() { return _instance; }

    void write(const Meta &meta, Data &&data)
    {
        // this function will be called from producers threads
        _queue.push({meta, std::move(data)});
    }
};

template <template <class T, size_t> class q> Processor<q> *Processor<q>::_instance = nullptr;

} // namespace logger
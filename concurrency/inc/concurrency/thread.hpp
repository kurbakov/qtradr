#pragma once

#include <cstdint>
#include <functional>
#include <pthread.h>

namespace concurrency
{

static constexpr size_t ThreadNameLen = 64;

class Task
{
public:
    std::function<void *(void *)> *execute;
    void *args;
};

struct ThreadParams
{
    // name of the thread, mostly for logging/debugging
    char threadName[ThreadNameLen];

    // cpu ID where the thread will be pinned
    int32_t cpuId;

    // supported PTHREAD_CREATE_DETACHED, PTHREAD_CREATE_JOINABLE
    int32_t detach_state;

    // Task to execute
    Task task;
};

class Thread
{
public:
    explicit Thread(ThreadParams &params) : m_params(params), m_state(State::NONE) {}

    ~Thread()
    {
        if (m_state > State::NONE)
        {
            pthread_attr_destroy(&m_attr);
        }
    }

    int init()
    {
        int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        if (m_params.cpuId > 0 && m_params.cpuId > numCPU)
        {
            // handle error
            // CPU is in invalid range
            return -1;
        }

        if (m_params.detach_state != PTHREAD_CREATE_JOINABLE && m_params.detach_state != PTHREAD_CREATE_DETACHED)
        {
            // handle error
            // invalid detach state
            return -1;
        }

        if (0 != pthread_attr_init(&m_attr))
        {
            // handle error
            return -1;
        }

        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(m_params.cpuId, &cpu_set);

        if (0 != pthread_attr_setaffinity_np(&m_attr, sizeof(cpu_set_t), &cpu_set))
        {
            // handle error
            // on failure errno: see
            // https://www.man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html
        }

        if (0 != pthread_attr_setdetachstate(&m_attr, m_params.detach_state))
        {
            // handle error
            return -1;
        }

        m_state = State::ATTR_INIT;
        return 0;
    }

    static int get_thread_name(char *buffer, size_t buff_len)
    {
        return pthread_getname_np(pthread_self(), buffer, buff_len);
    }

    int run()
    {
        int rc = pthread_create(&m_thread, &m_attr, &execute, &m_params);
        if (0 == rc)
        {
            m_state = State::RUNNING;
        }
        else {
            m_state = State::FAILED;
        }

        return rc;
    }

    int kill(int sig) { return pthread_kill(m_thread, sig); }

    int join(void **retval) const
    {
        if (m_params.detach_state != PTHREAD_CREATE_JOINABLE)
        {
            // can't join thread that is detached
            return -1;
        }

        return pthread_join(m_thread, retval);
    }

private:
    // a wrapper to bypass pthread limitations
    enum class State : uint8_t
    {
        NONE = 0,
        ATTR_INIT,
        RUNNING,
        DETACHED,
        KILL,
        FAILED,
    };

    static void *execute(void *task)
    {
        auto *params = reinterpret_cast<ThreadParams *>(task);
        pthread_setname_np(pthread_self(), params->threadName);

        return (*params->task.execute)(params->task.args);
    }

    ThreadParams m_params;
    State m_state;
    pthread_t m_thread;
    pthread_attr_t m_attr;
};
} // namespace concurrency
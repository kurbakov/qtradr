#pragma once


namespace concurrency
{
/**
 * @brief The lock free queue with single producre and single consumer.
 * Queue is using a ring buffer under the hood. The max number of elements
 * in the queue is limited by size of the ring buffer.
 */
class LFQueueSPSC{
public:
private:
};
} // ns concurrency

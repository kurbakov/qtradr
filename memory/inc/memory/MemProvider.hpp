#pragma once

#include <atomic>
#include <sys/mman.h>
#include <cstring>
#include <iostream>

namespace memory
{

static constexpr size_t Byte = 1;
static constexpr size_t KByte = 1024 * Byte;
static constexpr size_t MByte = 1024 * KByte;
static constexpr size_t GByte = 1024 * MByte;


/**
 * @brief The MemProvider class
 *
 * Shoul be used to provide big chunks of memory for the custom allocators.
 */
class MemProvider {
public:
    MemProvider() : _allocated{0} {}

    ~MemProvider()
    {
        if (_allocated.load(std::memory_order_relaxed))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << "During the life cycle, we still have unmapped: "
                      << _allocated << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << "Check the code on the memory leaks!!!" << std::endl;
        }
    }

    /**
     * @brief allocate
     *
     * @param size: amount of memory to allocate in bytes
     * @param prot: protection flags, see man mmap for the full list of flags.
     *  Default: PROT_READ | PROT_WRITE
     * @param flags: determines whether updates to the mapping are visible to other
     *  processes mapping the same region. Default: MAP_ANONYMOUS | MAP_PRIVATE
     *
     * @return pointer to the memory or nullptr if fails
     */
    void* allocate(size_t size, int prot = PROT_READ | PROT_WRITE, int flags = MAP_ANONYMOUS | MAP_PRIVATE)
    {
        void* mem = mmap(NULL, size, prot, flags, -1, 0);

        if (mem == MAP_FAILED)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << "Failed to allocate memory of size: "
                      << size << std::endl;
            std::cerr << __FILE__ << ":" << __LINE__ << std::strerror(errno) << std::endl;
            mem = nullptr;
        }

        _allocated.fetch_add(size, std::memory_order_acquire);
        return mem;
    }

    /**
     * @brief deallocate Free the memory that was previously allocated via MemProvider::allocate()
     *
     * @param mem: address of the memory to free
     * @param size: amount of bytes to free
     */
    void deallocate(void* mem, size_t size)
    {
        if (nullptr == mem || 0 == size) [[unlikely]]
        {
            std::cerr << __FILE__ << ":" << __LINE__ << "memory address is invalid or size is 0. memory addr: "
                      << mem << " and size: " << size << std::endl;
            return;
        }

        // potentially can fail, but what we suppose to do if it fails?
        // so let's pretend it never fails and all is good!
        if(-1 == munmap(mem, size))
        {
            std::cerr << __FILE__ << ":" << __LINE__ << "Failed to unmap the memory. memory addr: "
                      << mem << " and size: " << size << std::endl;
            return;
        }

        _allocated.fetch_sub(size, std::memory_order_acquire);
    }

private:
    // to track how much memory we allocated and deallocated
    // this is used to detect the memory leak
    std::atomic<ssize_t> _allocated;
};

} // ns memory

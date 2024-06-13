#include "memory/mem_provider.hpp"

namespace memory
{

void* MemProvider::allocate(size_t size, int prot, int flags)
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

void MemProvider::deallocate(void* mem, size_t size)
{
    if (nullptr == mem || 0 == size) [[unlikely]]
    {
        std::cerr << __FILE__ << ":" << __LINE__ << "memory address is invalid or size is 0. memory addr: "
                  << mem << " and size: " << size << std::endl;
        return;
    }

    // potentially can fail, but what we suppose to do if it fails?
    // so let's pretend it never fails and all is good!
    if(-1 == munmap(mem, size)) [[unlikely]]
    {
        std::cerr << __FILE__ << ":" << __LINE__ << "Failed to unmap the memory. memory addr: "
                  << mem << " and size: " << size << std::endl;
        return;
    }

    _allocated.fetch_sub(size, std::memory_order_acquire);
}

} // ns memory

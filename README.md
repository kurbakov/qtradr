# qtradr

Collection of libs and tools for low latency applications

## Plan for coding

- [ ] Network

  - [x] [UDP](./network/inc/network/udp/README.md) Server/Client
  - [x] [TCP](./network/inc/network/tcp/README.md) Server/Client
  - [ ] multicast
  - [ ] broadcast
  - [ ] HTTP Client
  - [ ] WebSocket Client
  - [ ] RestAPI Client
  - [ ] Tls (OpenSSL)

- [ ] Multiplexing

  - [ ] select
  - [ ] poll
  - [ ] epoll
  - [ ] io_uring

- [ ] Concurrency

  - [ ] Lock free queue (SPSC, MPSC, SPMC)
  - [ ] Lock free stack
  - [x] [Spinlock](./concurrency/inc/concurrency/Spinlock.hpp)
  - [ ] POSIX Thread pool with affinity

- [ ] [Memory](https://en.wikipedia.org/wiki/Memory_management)

  - [ ] arena allocator
  - [ ] stack allocator
  - [ ] slab allocator
  - [ ] buddy allocator
  - [ ] fixed-size blocks allocator
  - [ ] ...

- [ ] IPC

  - [ ] Shared memory
  - [ ] Signal

- [ ] Timers

  - [ ] system clock vs CPU time
  - [ ] watchdog

- [ ] OS (bash script with all linux config related setup)

  - [ ] CPU isolation
  - [ ] memory configuration
  - [ ] CPU set fix clock speed
  - [ ] ... other OS level optimization

- [ ] CI

  - [ ] Latency profile
  - [ ] Jitter stats
  - [ ] address sanitiser
  - [ ] thread sanitiser
  - [ ] unit tests
  - [ ] behave tests

- [ ] Logger

  - [ ] low latency logger

## Useful links

- [Cpp reference](https://en.cppreference.com/w/)
- [Godbolt](https://godbolt.org/)
- [Compiler flags](https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html)
- [LLVM](https://llvm.org/docs/CommandGuide/llvm-mca.html)
- [Toml](https://toml.io/en/)

## Libs

- [Benchmark](https://github.com/google/benchmark)
- [GTest](https://google.github.io/googletest/)
- [OpenSSL](https://www.openssl.org/docs/)
- [onload](https://github.com/Xilinx-CNS/onload)
- [behave](https://behave.readthedocs.io/en/stable/)

## Tools

- [valgrind](https://valgrind.org/)
- [perf](https://perf.wiki.kernel.org/index.php/Main_Page)
- [strace](https://strace.io/)
- [tcpdump](https://www.tcpdump.org/)
- [Clang-format](https://clang.llvm.org/docs/ClangFormat.html)

## Network

- [UDP](https://www.rfc-editor.org/rfc/rfc768)
- [TCP](https://www.rfc-editor.org/rfc/rfc761)
- [IP](https://www.rfc-editor.org/rfc/rfc760)
- [WebSocket](https://www.rfc-editor.org/rfc/rfc6455)
- [HTTP](https://www.rfc-editor.org/rfc/rfc2616)

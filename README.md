# qtradr

Collection of libs and tools for low latency applications

## About

This repo is a mix of documentation and implementation.

It is a cmake project with dependencies defined in [.cmake](./.cmake/)


To build the project:

```bash
cd qtradr
mkdir .build-release && cd .build-release
cmake .. && make -j8
ctest .
```

We also have benchmark tests, build and run it based on the need.
All benchmark tests in the repo have `*.bt.cpp` extension.


## Plan for coding

- [ ] [Network](./network/README.md)

  - [x] [UDP Server](./network/inc/network/udp/server.hpp)
  - [x] [UDP Client](./network/inc/network/udp/client.hpp)
  - [x] [TCP Server](./network/inc/network/tcp/server.hpp)
  - [x] [TCP Client](./network/inc/network/tcp/client.hpp)
  - [ ] WIP multicast [send](./network/inc/network/udp/mcast_send.hpp)/[recv](./network/inc/network/udp/mcast_recv.hpp)
  - [ ] WIP broadcast [send](./network/inc/network/udp/bcast_send.hpp)/[recv](./network/inc/network/udp/bcast_recv.hpp)
  - [ ] HTTP Client
  - [ ] WebSocket Client
  - [ ] RestAPI Client
  - [ ] Tls (OpenSSL)

- [ ] [Multiplexing](./multiplexing/README.md)

  - [x] [select vs. poll vs. epoll](./multiplexing/README.md)
  - [x] WIP [epoll](./multiplexing/inc/multiplexing/epoll.hpp)
  - [ ] io_uring

- [ ] [Concurrency](./concurrency/REAME.md)

  - [x] [Spinlock](./concurrency/inc/concurrency/spinlock.hpp)
  - [x] [Lock free bounded queue SPSC](./concurrency/inc/concurrency/lfqueue/ring/spsc.hpp)
  - [x] [Lock free bounded queue MPMC](./concurrency/inc/concurrency/lfqueue/ring/mpmc.hpp)
  - [x] [Lock free unbounded queue SPSC](./concurrency/inc/concurrency/lfqueue/list/spsc.hpp)
  - [ ] Lock free queue MPSC
  - [ ] Lock free queue SPMC
  - [ ] Lock free stack
  - [ ] POSIX Thread pool with affinity

- [ ] [Memory](./memory/REAME.md)

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

- [ ] misc

  - [x] [Config](./misc/config/README.md)
  - [ ] low latency logger

- [ ] Trading

  - [ ] OrderBook
  - [ ] Exchange protocol
    - [ ] Coinbase

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

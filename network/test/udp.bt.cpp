#include <benchmark/benchmark.h>

#include "network/udp/UdpServer.hpp"

static void BM_ServerInitDeinit(benchmark::State& state) {
    network::UDPServer server(98981);

    for (auto _ : state)
    {
        server.init();
        server.deinit();
    }
}
BENCHMARK(BM_ServerInitDeinit);

BENCHMARK_MAIN();

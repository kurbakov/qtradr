#include <benchmark/benchmark.h>

#include "network/udp/UdpServer.hpp"

static void BM_UdpServerInitDeinit(benchmark::State& state) {
    network::UDPServer server(98981);

    for (auto _ : state)
    {
        server.init();
        server.deinit();
    }
}
BENCHMARK(BM_UdpServerInitDeinit);

BENCHMARK_MAIN();

#include <benchmark/benchmark.h>

#include "network/udp/server.hpp"

static void BM_UdpServerInitDeinit(benchmark::State& state) {
    network::udp::UDPServer server(98981);

    for (auto _ : state)
    {
        server.init();
        server.deinit();
    }
}
BENCHMARK(BM_UdpServerInitDeinit);

#include <benchmark/benchmark.h>

#include "network/tcp/server.hpp"

static void BM_TcpServerInitDeinit(benchmark::State& state) {
    network::tcp::TCPServer server(98981);

    for (auto _ : state)
    {
        server.init();
        server.deinit();
    }
}
BENCHMARK(BM_TcpServerInitDeinit);

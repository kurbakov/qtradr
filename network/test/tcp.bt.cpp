#include <benchmark/benchmark.h>

#include "network/tcp/TcpServer.hpp"

static void BM_TcpServerInitDeinit(benchmark::State& state) {
    network::TCPServer server(98981);

    for (auto _ : state)
    {
        server.init();
        server.deinit();
    }
}
BENCHMARK(BM_TcpServerInitDeinit);

BENCHMARK_MAIN();

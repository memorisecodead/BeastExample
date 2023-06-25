#include "BeastBench.hpp"
#include "PocoBench.hpp"
#include "CurlBench.hpp"

const int iterations = 1000;

BENCHMARK(beast_test)->Arg(iterations)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(poco_test)->Arg(iterations)->UseManualTime()->Unit(benchmark::kMillisecond);
BENCHMARK(libcurl_test)->Arg(iterations)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

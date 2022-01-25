
#include <benchmark/benchmark.h>


static void BM_Malloc(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        for (int i = 0; i < state.range(1); i++)
        {
            auto* ptr = std::malloc(state.range(0));
            benchmark::DoNotOptimize(ptr);
            std::free(ptr);
        }
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_Malloc)->Ranges({ {8, 8 << 10}, {1, 512} });


static void BM_New(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        for (int i = 0; i < state.range(1); i++)
        {
            auto* ptr = new char[state.range(0)];
            benchmark::DoNotOptimize(ptr);
            delete[] ptr;
        }
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_New)->Ranges({ {8, 8 << 10}, {1, 512} });
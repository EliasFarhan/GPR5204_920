
#include <benchmark/benchmark.h>
#include "random_fill.h"


constexpr long fromRange = 8;
constexpr long toRange = 1 << 20;

static void BM_Copy(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);
    
    for (auto _ : state)
    {
        for(std::size_t i = 0; i < length;i++)
        {
            numbers_dst[i] = numbers_src[i];
        }
    }
}

BENCHMARK(BM_Copy)->Range(fromRange, toRange);


static void BM_CopyMemcpy(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);

    for (auto _ : state)
    {
        std::memcpy(numbers_dst.data(), numbers_src.data(), sizeof(int) * length);
    }
}

static void BM_CopyAlgo(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);

    for (auto _ : state)
    {
        std::copy(numbers_src.begin(), numbers_src.end(), numbers_dst.begin());
    }
}

BENCHMARK(BM_CopyAlgo)->Range(fromRange, toRange);

BENCHMARK(BM_CopyMemcpy)->Range(fromRange, toRange);

// Copy if only odd
static void BM_CopyIf(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> numbers_src(length);
    std::vector<int> numbers_dst(length);

    fill_vector(numbers_src, 0, 100);

    for (auto _ : state)
    {
        std::copy_if(numbers_src.begin(), 
            numbers_src.end(), 
            numbers_dst.begin(), 
            [](auto n)
            {
                return n % 2 == 0;
            });
    }
}

BENCHMARK(BM_CopyIf)->Range(fromRange, toRange);
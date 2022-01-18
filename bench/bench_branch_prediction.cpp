#include <benchmark/benchmark.h>

#include "bench_utils.h"
#include "random_fill.h"


const long fromRange = 8;

const long toRange = 1 << 15;


static void BM_Branch_Not_Predicted(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> v1(length);
    std::vector<int> v2(length);
    std::vector<std::uint8_t> c1(length);
    for(std::size_t i = 0; i < length; i++)
    {
        v1[i] = RandomRange(0, std::numeric_limits<int>::max());
        v2[i] = RandomRange(0, std::numeric_limits<int>::max());
        c1[i] = RandomRange(0, std::numeric_limits<int>::max()) & 0x1;
    }

    for (auto _ : state)
    {
        int a1 = 0, a2 = 0;
        for(std::size_t i = 0; i < length; i++)
        {
            if(c1[i])
            {
                a1 += v1[i];
            }
            else
            {
                a1 += v2[i];
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_Branch_Not_Predicted)->Arg(1 << 22);

static void BM_Branch_Predicted(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> v1(length);
    std::vector<int> v2(length);
    std::vector<std::uint8_t> c1(length);
    for (std::size_t i = 0; i < length; i++)
    {
        v1[i] = RandomRange(0, std::numeric_limits<int>::max());
        v2[i] = RandomRange(0, std::numeric_limits<int>::max());
        c1[i] = RandomRange(0, std::numeric_limits<int>::max()) >= 0;
    }

    for (auto _ : state)
    {
        int a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length; i++)
        {
            if (c1[i])
            {
                a1 += v1[i];
            }
            else
            {
                a1 += v2[i];
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_Branch_Predicted)->Arg(1 << 22);

static void BM_Branch_Predicted_Alt(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> v1(length);
    std::vector<int> v2(length);
    std::vector<std::uint8_t> c1(length);
    for (std::size_t i = 0; i < length; i++)
    {
        v1[i] = RandomRange(0, std::numeric_limits<int>::max());
        v2[i] = RandomRange(0, std::numeric_limits<int>::max());
        if (i == 0)
        {
            c1[i] = RandomRange(0, std::numeric_limits<int>::max()) >= 0;
        }
        else
        {
            c1[i] = !c1[i - 1];
        }
    }

    for (auto _ : state)
    {
        int a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length; i++)
        {
            if (c1[i])
            {
                a1 += v1[i];
            }
            else
            {
                a1 += v2[i];
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_Branch_Predicted_Alt)->Arg(1 << 22);

static void BM_Branch_False(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> v1(length);
    std::vector<int> v2(length);
    std::vector<std::uint8_t> c1(length);
    std::vector<std::uint8_t> c2(length);
    for (std::size_t i = 0; i < length; i++)
    {
        v1[i] = RandomRange(0, std::numeric_limits<int>::max());
        v2[i] = RandomRange(0, std::numeric_limits<int>::max());

        c1[i] = RandomRange(0, std::numeric_limits<int>::max()) & 0x1;
        c2[i] = !c1[i];
    }

    for (auto _ : state)
    {
        int a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length; i++)
        {
            if (c1[i] || c2[i])
            {
                a1 += v1[i];
            }
            else
            {
                a1 *= v2[i];
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_Branch_False)->Arg(1 << 22);

static void BM_Branch_False_BitWise(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<int> v1(length);
    std::vector<int> v2(length);
    std::vector<std::uint8_t> c1(length);
    std::vector<std::uint8_t> c2(length);
    for (std::size_t i = 0; i < length; i++)
    {
        v1[i] = RandomRange(0, std::numeric_limits<int>::max());
        v2[i] = RandomRange(0, std::numeric_limits<int>::max());

        c1[i] = RandomRange(0, std::numeric_limits<int>::max()) & 0x1;
        c2[i] = !c1[i];
    }

    for (auto _ : state)
    {
        int a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length; i++)
        {
            if (c1[i] | c2[i])
            {
                a1 += v1[i];
            }
            else
            {
                a1 *= v2[i];
            }
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_Branch_False_BitWise)->Arg(1 << 22);
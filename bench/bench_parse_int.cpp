
#include <benchmark/benchmark.h>
#include <vector>
#include <random>

#include <bench_utils.h>

#include "random_fill.h"

#include "parse_int.h"

const long fromRange = 8;

const long toRange = 1 << 22;




std::vector<std::string> generate_strings(std::size_t numbers)
{
    std::vector<std::string> v(numbers);
    for(auto& string_number: v)
    {
        string_number.resize(17);
        string_number[16] = 0;
        const auto int_count = RandomRange(0, 10);
        for(int i = 0; i < int_count; i++)
        {
            if(i == 0)
            {
                string_number[i] = RandomRange(0, 2) ? '-': RandomRange(0, 10) + '0';
            }
            else 
            {
                string_number[i] = RandomRange(0, 10) + '0';
            }
        }
        for(int i = int_count; i < 16; i++)
        {
            string_number[i] = RandomRange(-128, 127);
        }
    }
    return v;
}


static void BM_ParseIntNaive(benchmark::State& state)
{
    std::vector<std::string> v1 = generate_strings(state.range(0));
    for (auto _ : state)
    {
        int result = 0;
        for (std::size_t i = 0; i < state.range(0); i++)
        {
            result += parse_int(v1[i].data());
        }
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_ParseIntNaive)->Range(fromRange, toRange);

static void BM_ParseIntBranchless(benchmark::State& state)
{
    std::vector<std::string> v1 = generate_strings(state.range(0));
    for (auto _ : state)
    {
        int result = 0;
        for (std::size_t i = 0; i < state.range(0); i++)
        {
            result +=parse_int_branchless(v1[i].data());
        }
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_ParseIntBranchless)->Range(fromRange, toRange);

static void BM_ParseIntAtoi(benchmark::State& state)
{
    std::vector<std::string> v1 = generate_strings(state.range(0));
    for (auto _ : state)
    {
        int result = 0;
        for (std::size_t i = 0; i < state.range(0); i++)
        {
            result += std::atoi(v1[i].data());
        }
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_ParseIntAtoi)->Range(fromRange, toRange);
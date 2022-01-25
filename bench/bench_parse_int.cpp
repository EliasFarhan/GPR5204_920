
#include <benchmark/benchmark.h>
#include <vector>
#include <random>

#include <bench_utils.h>

#include "random_fill.h"

const long fromRange = 8;

const long toRange = 1 << 15;



std::uint32_t parse_int(const char* p)
{
    std::uint32_t val = 0;
    while (true)
    {
        int n = *p;
        if (n >= '0' && n <= '9')
        {
            val *= 10;
            val += n - '0';
            p++;
        }
        else
            break;
    }
    return val;
}

std::uint32_t parse_int_branchless(const char* p)
{
    std::uint32_t val = 0;
    int is_digit = 1;
    while (is_digit)
    {
        int n = *p;
        is_digit = (n >= '0') & (n <= '9');
        const std::uint32_t new_val = val * 10 + (n - '0');
        val = val + is_digit * new_val;
        p++;
    }
    return val;
}

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
            string_number[i] = RandomRange(0, 10) + '0';
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
        for (std::size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(parse_int(v1[i].data()));
        }
    }
}

BENCHMARK(BM_ParseIntNaive)->Range(fromRange, toRange);

static void BM_ParseIntBranchless(benchmark::State& state)
{
    std::vector<std::string> v1 = generate_strings(state.range(0));
    for (auto _ : state)
    {
        for (std::size_t i = 0; i < state.range(0); i++)
        {
            benchmark::DoNotOptimize(parse_int_branchless(v1[i].data()));
        }
    }
}

BENCHMARK(BM_ParseIntBranchless)->Range(fromRange, toRange);

#include <benchmark/benchmark.h>
#include <vector>
#include <random>

#include <bench_utils.h>

#include "random_fill.h"

const long fromRange = 8;

const long toRange = 1 << 15;



int parse_int(const char* p)
{

    int sign = 1;
    if(*p == '-')
    {
        sign = -1;
        p++;
    }
    int val = 0;
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
    return val * sign;
}

int parse_int_branchless(const char* p)
{
    static constexpr int sign_array[2] = {-1, 1};
    int conditional = *p == '-';
    int sign = sign_array[conditional];
    if(sign == -1)
    {
        p++;
    }
    //p = p + conditional;
    int val = 0;
    int is_digit = 1;
    int new_val = 0;
    while (is_digit)
    {
        val = new_val;
        int n = *p;
        new_val = val * 10 + (n - '0');
        p++;
        is_digit = (n >= '0') && (n <= '9');
    }
    return val * sign;
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
            if(i == 0)
            {
                string_number[i] = RandomRange(0, 1) ? '-': RandomRange(0, 10) + '0';
            }
            else {
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

#include "shape.h"
#include <benchmark/benchmark.h>
#include <memory>
#include <vector>

#include "random_fill.h"


const long fromRange = 8;

const long toRange = 1 << 22;


static void BM_01_Vtable(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<std::unique_ptr<Shape>> v1;
    v1.reserve(length);
    for (std::size_t i = 0; i < length/2; i++)
    {
        v1.push_back(std::make_unique<Circle>(RandomRange(0.0f, 100.0f)));
        v1.push_back(std::make_unique<Rect>(RandomRange(0.0f, 100.0f), RandomRange(0.0f, 100.0f)));
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v1.begin(), v1.end(), g);

    for (auto _ : state)
    {
        float a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length; i++)
        {
            a1 += v1[i]->Area();
            a2 += v1[i]->Perimeter();
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_01_Vtable)->Range(fromRange, toRange);


static void BM_01_No_Vtable(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<std::unique_ptr<Circle>> v1;
    v1.reserve(length / 2);
    std::vector<std::unique_ptr<Rect>> v2;
    v2.reserve(length / 2);
    for (std::size_t i = 0; i < length / 2; i++)
    {
        v1.push_back(std::make_unique<Circle>(RandomRange(0.0f, 100.0f)));
        v2.push_back(std::make_unique<Rect>(RandomRange(0.0f, 100.0f), RandomRange(0.0f, 100.0f)));
    }

    for (auto _ : state)
    {
        float a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length/2; i++)
        {
            a1 += v1[i]->Area();
            a2 += v1[i]->Perimeter();
        }
        for (std::size_t i = 0; i < length / 2; i++)
        {
            a1 += v2[i]->Area();
            a2 += v2[i]->Perimeter();
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_01_No_Vtable)->Range(fromRange, toRange);


static void BM_01_No_Vtable_Val(benchmark::State& state)
{
    const std::size_t length = state.range(0);
    std::vector<Circle> v1;
    v1.reserve(length / 2);
    std::vector<Rect> v2;
    v2.reserve(length / 2);
    for (std::size_t i = 0; i < length / 2; i++)
    {
        v1.emplace_back(RandomRange(0.0f, 100.0f));
        v2.emplace_back(RandomRange(0.0f, 100.0f), RandomRange(0.0f, 100.0f));
    }

    for (auto _ : state)
    {
        float a1 = 0, a2 = 0;
        for (std::size_t i = 0; i < length / 2; i++)
        {
            a1 += v1[i].Area();
            a2 += v1[i].Perimeter();
        }
        for (std::size_t i = 0; i < length / 2; i++)
        {
            a1 += v2[i].Area();
            a2 += v2[i].Perimeter();
        }
        benchmark::DoNotOptimize(a1);
        benchmark::DoNotOptimize(a2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(length * state.iterations());

}

BENCHMARK(BM_01_No_Vtable_Val)->Range(fromRange, toRange);